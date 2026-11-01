// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../TimeoutMonitor.h"
#include "../Task.h"
#include <chrono>
#include <thread>

class MockTask : public Task {
public:
    MockTask(const std::string& name, const std::string& deadline) 
        : Task(name, deadline) {}
    int totalTime() const override { return 0; }
    void execute() override {}
    bool isLeaf() const override { return true; }
    Task* clone() const override { return new MockTask(*this); }
    TaskIterator* createIterator(int /*level*/) override { return nullptr; }
};

TEST_CASE("TimeoutMonitor Thread Lifecycle and Shutdown") {
    MockTask task("TestTask", "2026-12-31");

    SUBCASE("Monitor shuts down cleanly without waiting 30 seconds") {
        auto start_time = std::chrono::steady_clock::now();
        
        {
            TimeoutMonitor monitor(&task); 
            
            monitor.finished();
        } 
        
        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
        
        CHECK(duration < 1);
    }

    SUBCASE("Monitor update() resets the timer without blocking") {
        TimeoutMonitor monitor(&task);
        
        monitor.update();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        monitor.update(); // Call it a second time
    }
}