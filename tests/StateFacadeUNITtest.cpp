#include "doctest.h"
#include <iostream>
#include <sstream>
#include <string>
#include "../SimpleTask.h"
#include "../TaskGroup.h"
#include "../TaskState.h"
#include "../TaskSnapshotManager.h"
#include "../TividyFacade.h"

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

