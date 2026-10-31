#include "doctest.h"
#include <iostream>
#include <sstream>
#include <string>
#include "../SimpleTask.h"
#include "../TaskGroup.h"
#include "../TaskState.h"
#include "../TaskSnapshotManager.h"
#include "../TividyFacade.h"
#include "../Target.h"
#include <fstream>

template <typename F>
static std::string quiet(F f) {
    std::ostringstream captured;
    std::streambuf* old = std::cout.rdbuf(captured.rdbuf());
    f();
    std::cout.rdbuf(old);
    return captured.str();
}

static Task* sampleTree() {
    TaskGroup* g = new TaskGroup("0.Project", "2026-12-31");
    g->add(new SimpleTask("1.Triage", "d", 10));
    g->add(new SimpleTask("4.Exam", "d", 20));
    g->add(new SimpleTask("7.Admit", "d", 5));
    return g;
}

TEST_CASE("State: a new task is Pending and not finished") {
    SimpleTask t("1.Triage", "d", 10);
    CHECK(t.getStateName() == "Pending");
    CHECK(!t.isFinished());
}

TEST_CASE("State: happy path Pending -> InProgress -> Completed") {
    SimpleTask t("1.Triage", "d", 10);
    t.start();
    CHECK(t.getStateName() == "InProgress");
    t.complete();
    CHECK(t.getStateName() == "Completed");
    CHECK(t.isFinished());
}

TEST_CASE("State: every allowed transition") {
    SimpleTask t("1.T", "d", 1);
    SUBCASE("reject then reopen") {
        t.start(); t.reject();
        CHECK(t.getStateName() == "Rejected");
        CHECK(!t.isFinished());
        t.reopen();
        CHECK(t.getStateName() == "Pending");
    }
    SUBCASE("timeout then start again") {
        t.start(); t.timeout();
        CHECK(t.getStateName() == "TimedOut");
        t.start();
        CHECK(t.getStateName() == "InProgress");
    }
    SUBCASE("cancel is final and counts as finished") {
        t.cancel();
        CHECK(t.getStateName() == "Cancelled");
        CHECK(t.isFinished());
        CHECK_THROWS_AS(t.start(), InvalidTransition);
        CHECK_THROWS_AS(t.reopen(), InvalidTransition);
    }
    SUBCASE("completed can be reopened for rework") {
        t.start(); t.complete(); t.reopen();
        CHECK(t.getStateName() == "Pending");
    }
}

TEST_CASE("State: invalid transitions throw and leave the state unchanged") {
    SimpleTask t("1.T", "d", 1);
    CHECK_THROWS_AS(t.complete(), InvalidTransition);
    CHECK_THROWS_AS(t.reject(), InvalidTransition);
    CHECK_THROWS_AS(t.timeout(), InvalidTransition);
    CHECK_THROWS_AS(t.reopen(), InvalidTransition);
    CHECK(t.getStateName() == "Pending");
    t.start();
    CHECK_THROWS_AS(t.start(), InvalidTransition);
    CHECK(t.getStateName() == "InProgress");
}

TEST_CASE("State: clone gets its own copy of the state") {
    SimpleTask t("1.T", "d", 1);
    t.start();
    Task* copy = t.clone();
    t.complete();
    CHECK(copy->getStateName() == "InProgress");
    CHECK(t.getStateName() == "Completed");
    delete copy;
}

TEST_CASE("State: snapshot restore brings the state back") {
    SimpleTask t("1.T", "d", 1);
    TaskSnapshotManager m;
    m.addSnapshot(&t);
    t.start();
    t.complete();
    CHECK(m.restoreLatest(&t));
    CHECK(t.getStateName() == "Pending");
}

TEST_CASE("State: Completed and Cancelled count as finished, the others do not") {
    SimpleTask done("1.A", "d", 1), cancelled("1.B", "d", 1), rejected("1.C", "d", 1);
    done.start(); done.complete();
    cancelled.cancel();
    rejected.start(); rejected.reject();
    CHECK(done.isFinished());
    CHECK(cancelled.isFinished());
    CHECK(!rejected.isFinished());
}

TEST_CASE("Facade: runWorkflow completes every task through the chain") {
    TividyFacade f;
    f.setRoot(sampleTree());
    std::string out = quiet([&] { f.runWorkflow(); });
    CHECK(f.getTaskStatus("1.Triage") == "Completed");
    CHECK(f.getTaskStatus("4.Exam") == "Completed");
    CHECK(f.getTaskStatus("7.Admit") == "Completed");
    CHECK(out.find("Workflow finished.") != std::string::npos);
}

TEST_CASE("Facade: lifecycle actions, invalid moves and undo") {
    TividyFacade f;
    f.setRoot(sampleTree());
    quiet([&] {
        CHECK(f.startTask("1.Triage"));
        CHECK(!f.startTask("1.Triage"));
        CHECK(f.rejectTask("1.Triage"));
    });
    CHECK(f.getTaskStatus("1.Triage") == "Rejected");
    quiet([&] { CHECK(f.undoLast()); });
    CHECK(f.getTaskStatus("1.Triage") == "InProgress");
    quiet([&] { CHECK(f.undoLast()); });
    CHECK(f.getTaskStatus("1.Triage") == "Pending");
    std::string out = quiet([&] { CHECK(!f.undoLast()); });
    CHECK(out.find("Nothing to undo") != std::string::npos);
}

TEST_CASE("Facade: completing a pending task runs and logs the command") {
    TividyFacade f;
    f.setRoot(sampleTree());
    quiet([&] { CHECK(f.completeTask("4.Exam")); });
    CHECK(f.getTaskStatus("4.Exam") == "Completed");
    std::string log = quiet([&] { f.showLog(); });
    CHECK(log.find("(log is empty)") == std::string::npos);
    quiet([&] { CHECK(f.undoLast()); });
    CHECK(f.getTaskStatus("4.Exam") == "Pending");
}

TEST_CASE("Facade: a task already in progress is refused by the chain and rolled back") {
    TividyFacade f;
    f.setRoot(sampleTree());
    quiet([&] { f.startTask("7.Admit"); });
    std::string out = quiet([&] { f.runWorkflow(); });
    CHECK(out.find("Cannot start a task that is InProgress") != std::string::npos);
    CHECK(f.getTaskStatus("1.Triage") == "Completed");
    CHECK(f.getTaskStatus("7.Admit") == "InProgress");
}

TEST_CASE("Facade: checkTimeouts with nothing timed out") {
    TividyFacade f;
    f.setRoot(sampleTree());
    std::string out = quiet([&] { f.checkTimeouts(); });
    CHECK(out.find("No timed-out tasks.") != std::string::npos);
}

TEST_CASE("Facade: unknown tasks and no workflow") {
    TividyFacade f;
    CHECK(f.getTaskStatus("nope") == "Task not found");
    std::string out = quiet([&] { f.runWorkflow(); CHECK(!f.loadWorkflow("x.txt")); });
    CHECK(out.find("No workflow loaded.") != std::string::npos);
    f.setRoot(sampleTree());
    quiet([&] { CHECK(!f.startTask("nope")); });
}

static void buildSampleWorkflow(TividyFacade& f) {
    f.beginWorkflow("0.Intake", "2026-11-02");
    f.addTask("1.Triage", "30", 1, 15);
    f.beginGroup("0.Assessment", "60");
    f.addTask("4.Exam", "45", 4, 20);
    f.endGroup();
    f.addTask("7.Admit", "90", 7, 10);
    f.finishWorkflow();
}

static int countClosedEntries() {
    std::ifstream file("workflow.log");
    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.find("WORKFLOW CLOSED") != std::string::npos) {
            count++;
        }
    }
    return count;
}

class SpyNotifier : public Target {
public:
    int projectEmails;
    std::string lastRecipient;
    SpyNotifier() : projectEmails(0) {}
    void notifyTaskComplete(Task*, std::string) override {}
    void notifyProjectComplete(Task*, std::string, std::string recipient) override {
        projectEmails++;
        lastRecipient = recipient;
    }
    void notifyStateChange(Task*, const std::string&, const std::string&) override {}
};

TEST_CASE("Facade: tasks are created in order through the Director") {
    TividyFacade f;
    buildSampleWorkflow(f);
    CHECK(f.getTaskStatus("0.Intake") == "Pending");
    CHECK(f.getTaskStatus("1.Triage") == "Pending");
    CHECK(f.getTaskStatus("4.Exam") == "Pending");
    CHECK(f.getTaskStatus("7.Admit") == "Pending");
}

TEST_CASE("Facade: task creation refuses calls made out of order") {
    TividyFacade f;
    quiet([&] {
        CHECK(!f.addTask("1.Triage", "30", 1, 15));
        CHECK(!f.beginGroup("0.Group", "30"));
        CHECK(!f.endGroup());
        CHECK(!f.finishWorkflow());
        CHECK(f.beginWorkflow("0.Intake", "2026-11-02"));
        CHECK(f.beginGroup("0.Group", "30"));
        CHECK(!f.finishWorkflow());
    });
}

TEST_CASE("Facade: a created workflow runs and closes the log once") {
    int before = countClosedEntries();
    TividyFacade f;
    buildSampleWorkflow(f);
    quiet([&] { f.runWorkflow(); });
    CHECK(f.getTaskStatus("1.Triage") == "Completed");
    CHECK(f.getTaskStatus("4.Exam") == "Completed");
    CHECK(f.getTaskStatus("7.Admit") == "Completed");
    CHECK(countClosedEntries() == before + 1);
}

TEST_CASE("Facade: the log closes when the last task is not on the final handler") {
    int before = countClosedEntries();
    TividyFacade f;
    f.beginWorkflow("0.Intake", "2026-11-02");
    f.addTask("1.Triage", "30", 1, 15);
    f.addTask("4.Exam", "45", 4, 20);
    f.finishWorkflow();
    quiet([&] { f.runWorkflow(); });
    CHECK(countClosedEntries() == before + 1);
}

TEST_CASE("Facade: undo still works after running a level") {
    TividyFacade f;
    buildSampleWorkflow(f);
    quiet([&] { f.runWorkflow(); });
    quiet([&] { CHECK(f.undoLast()); });
    CHECK(f.getTaskStatus("7.Admit") == "Pending");
}

TEST_CASE("Facade: finishing a workflow sends one email to the chosen address") {
    TividyFacade f;
    CHECK(!f.setNotificationEmail("student@example.com"));
    SpyNotifier* spy = new SpyNotifier();
    f.setNotifier(spy);
    CHECK(f.setNotificationEmail("student@example.com"));
    CHECK(!f.setNotificationEmail(""));
    buildSampleWorkflow(f);
    quiet([&] { f.runWorkflow(); f.runWorkflow(); });
    CHECK(spy->projectEmails == 1);
    CHECK(spy->lastRecipient == "student@example.com");
}

TEST_CASE("Facade: finishing the last task through a facade action sends one email") {
    TividyFacade f;
    SpyNotifier* spy = new SpyNotifier();
    f.setNotifier(spy);
    CHECK(f.setNotificationEmail("student@example.com"));
    f.beginWorkflow("0.Intake", "2026-11-02");
    f.addTask("1.Triage", "30", 1, 15);
    f.addTask("4.Exam", "45", 4, 20);
    f.finishWorkflow();
    quiet([&] {
        f.completeTask("1.Triage");
        CHECK(spy->projectEmails == 0);
        f.cancelTask("4.Exam");
        f.runWorkflow();
    });
    CHECK(spy->projectEmails == 1);
}

TEST_CASE("Facade: the log closes when a facade action finishes the last task") {
    int before = countClosedEntries();
    TividyFacade f;
    f.beginWorkflow("0.Intake", "2026-11-02");
    f.addTask("1.Triage", "30", 1, 15);
    f.addTask("4.Exam", "45", 4, 20);
    f.finishWorkflow();
    quiet([&] {
        CHECK(f.completeTask("1.Triage"));
        CHECK(f.cancelTask("4.Exam"));
    });
    CHECK(countClosedEntries() == before + 1);
}

TEST_CASE("Facade: actions are logged and showLog keeps them in the file") {
    TividyFacade f;
    buildSampleWorkflow(f);
    quiet([&] {
        f.startTask("1.Triage");
        f.rejectTask("1.Triage");
        f.reopenTask("1.Triage");
        f.cancelTask("4.Exam");
    });
    std::string first = quiet([&] { f.showLog(); });
    CHECK(first.find("STARTED: 1.Triage") != std::string::npos);
    CHECK(first.find("REJECTED: 1.Triage") != std::string::npos);
    CHECK(first.find("REOPENED: 1.Triage") != std::string::npos);
    CHECK(first.find("CANCELLED: 4.Exam") != std::string::npos);
    std::string second = quiet([&] { f.showLog(); });
    CHECK(second.find("STARTED: 1.Triage") != std::string::npos);
}

TEST_CASE("Facade: addTask refuses a level no handler can take") {
    TividyFacade f;
    quiet([&] {
        f.beginWorkflow("0.Intake", "2026-11-02");
        CHECK(!f.addTask("0.Nothing", "30", 0, 15));
        CHECK(!f.addTask("10.Nothing", "30", 10, 15));
        CHECK(f.addTask("9.Last", "30", 9, 15));
    });
}
