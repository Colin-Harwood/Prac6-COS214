#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../Command.h"
#include "../ExecuteCommand.h"
#include "../LogCommand.h"
#include "../LogQueue.h"
#include "../TaskSnapshotManager.h"

#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>


class MockTask : public Task {
public:
    std::string name;
    bool started = false;
    bool completed = false;
    int restoreCount = 0;
    bool shouldThrowOnStart = false;

    explicit MockTask(const std::string& n) : Task(n, ""), name(n) {}

    // Copy constructor for clone()
    MockTask(const MockTask& other)
        : Task(other), name(other.name), started(other.started),
          completed(other.completed), restoreCount(other.restoreCount),
          shouldThrowOnStart(other.shouldThrowOnStart) {}

    std::string getName() const { return name; }

    void start() override {
        if (shouldThrowOnStart) throw std::runtime_error("Start failed");
        started = true;
    }

    void complete() override { completed = true; }

    Task* clone() const override {
        return new MockTask(*this);
    }

    void restoreFrom(Task* /*t*/) override {
        restoreCount++;
        started = false;
        completed = false;
    }

    // Dummy implementations for Task pure virtuals
    bool isLeaf() const override { return true; }
    int totalTime() const override { return 0; }
    void execute() override {}
    TaskIterator* createIterator(int) override { return nullptr; }
};

TEST_CASE("LogQueue basic operations") {
    LogQueue queue;
    CHECK(queue.empty());

    // We need a concrete command to push. We'll use a dummy ExecuteCommand.
    TaskSnapshotManager snapMgr;
    Command* cmd = new ExecuteCommand(&queue, &snapMgr);

    queue.push(cmd);
    CHECK_FALSE(queue.empty());
}

TEST_CASE("LogQueue flushTo writes and clears") {
    LogQueue queue;
    TaskSnapshotManager snapMgr;

    // Create a mock task to record
    MockTask task("TestTask");
    Command* cmd = new ExecuteCommand(&queue, &snapMgr);
    cmd->execute(&task); // This will push a cloned command to the queue

    CHECK_FALSE(queue.empty());

    std::ostringstream oss;
    queue.flushTo(oss);

    std::string output = oss.str();
    CHECK(output.find("EXECUTED: TestTask") != std::string::npos);
    CHECK(queue.empty());
}

TEST_CASE("TaskSnapshotManager save and restore") {
    TaskSnapshotManager mgr;
    MockTask task("Original");
    task.started = true;

    mgr.save(&task);

    // Restore should reset the mock task's state
    bool restored = mgr.restoreLatest(&task);
    CHECK(restored);
    CHECK(task.restoreCount == 1);
    CHECK(task.started == false);
}

TEST_CASE("TaskSnapshotManager restore on empty stack") {
    TaskSnapshotManager mgr;
    MockTask task("Original");

    bool restored = mgr.restoreLatest(&task);
    CHECK_FALSE(restored);
}

TEST_CASE("ExecuteCommand execute success path") {
    LogQueue queue;
    TaskSnapshotManager snapMgr;
    ExecuteCommand cmd(&queue, &snapMgr);

    MockTask task("SuccessTask");
    cmd.execute(&task);

    CHECK(task.started);
    CHECK(task.completed);
    CHECK_FALSE(queue.empty()); // Should have logged "EXECUTED"
}

TEST_CASE("ExecuteCommand execute failure path") {
    LogQueue queue;
    TaskSnapshotManager snapMgr;
    ExecuteCommand cmd(&queue, &snapMgr);

    MockTask task("FailTask");
    task.shouldThrowOnStart = true;

    CHECK_THROWS_AS(cmd.execute(&task), std::runtime_error);

    // Should have logged "FAILED"
    CHECK_FALSE(queue.empty());

    // Snapshot should have been restored
    CHECK(task.restoreCount == 1);
}

TEST_CASE("ExecuteCommand undo") {
    LogQueue queue;
    TaskSnapshotManager snapMgr;
    ExecuteCommand cmd(&queue, &snapMgr);

    MockTask task("UndoTask");
    cmd.execute(&task); // Puts a snapshot in the manager

    cmd.undo(&task);

    CHECK(task.restoreCount == 1);
    // Should have logged "UNDONE"
    CHECK_FALSE(queue.empty());
}

TEST_CASE("ExecuteCommand clone") {
    LogQueue queue;
    TaskSnapshotManager snapMgr;
    ExecuteCommand cmd(&queue, &snapMgr);

    Command* cloned = cmd.clone();
    REQUIRE(cloned != nullptr);

    // Verify it's an independent copy
    MockTask task("CloneTask");
    cloned->execute(&task);
    CHECK(task.started);

    delete cloned;
}

TEST_CASE("LogCommand execute and flush to file") {
    const std::string testFile = "test_workflow.log";

    // Clean up from previous runs
    std::remove(testFile.c_str());

    LogQueue queue;
    TaskSnapshotManager snapMgr;
    LogCommand cmd(&queue, &snapMgr, testFile);

    // Pre-fill queue
    TaskSnapshotManager snapMgr2;
    queue.push(new ExecuteCommand(&queue, &snapMgr2));

    MockTask task("LogTask");
    cmd.execute(&task);

    // Check file was created and contains data
    std::ifstream inFile(testFile);
    REQUIRE(inFile.is_open());

    std::string line;
    std::getline(inFile, line);
    CHECK_FALSE(line.empty());

    inFile.close();
    std::remove(testFile.c_str()); // Cleanup
}

TEST_CASE("LogCommand undo is a no-op") {
    const std::string testFile = "test_workflow_noop.log";
    std::remove(testFile.c_str());

    LogQueue queue;
    TaskSnapshotManager snapMgr;
    LogCommand cmd(&queue, &snapMgr, testFile);

    MockTask task("NoopTask");
    // Should not throw or crash
    CHECK_NOTHROW(cmd.undo(&task));

    std::remove(testFile.c_str());
}

TEST_CASE("LogCommand clone") {
    const std::string testFile = "test_workflow_clone.log";
    std::remove(testFile.c_str());

    LogQueue queue;
    TaskSnapshotManager snapMgr;
    LogCommand cmd(&queue, &snapMgr, testFile);

    Command* cloned = cmd.clone();
    REQUIRE(cloned != nullptr);

    MockTask task("CloneLogTask");
    cloned->execute(&task);

    // Verify cloned command wrote to the same file
    std::ifstream inFile(testFile);
    REQUIRE(inFile.is_open());
    std::string line;
    std::getline(inFile, line);
    CHECK_FALSE(line.empty());

    inFile.close();
    delete cloned;
    std::remove(testFile.c_str());
}
