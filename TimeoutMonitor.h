#ifndef TIMEOUT_MONITOR
#define TIMEOUT_MONITOR

#include "Observer.h"
#include "Task.h"
#include "TimedOutState.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

class TimeoutMonitor {
private:
    using Clock = std::chrono::steady_clock;
    Task* task;
    Clock::time_point lastUpdateTime;
    std::thread worker_thread;
    std::mutex mtx;
    std::condition_variable cv;
    bool stopRequested = false;

    void clockWatcher();

public:
    TimeoutMonitor(Task* task);
    ~TimeoutMonitor();
    void update();
    void finished();
};


#endif