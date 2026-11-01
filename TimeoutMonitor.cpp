#include "TimeoutMonitor.h"

TimeoutMonitor::TimeoutMonitor(Task* task) : task(task), lastUpdateTime(Clock::now()) {
    worker_thread = std::thread(&TimeoutMonitor::clockWatcher, this);
}

void TimeoutMonitor::clockWatcher() {
    std::unique_lock<std::mutex> lock(mtx);
    
    while (!stopRequested) {
        auto timeout_time = lastUpdateTime + std::chrono::seconds(30);

        std::cv_status status = cv.wait_until(lock, timeout_time);

        if (stopRequested) {
            return;
        }

        if (status == std::cv_status::timeout) {
            if (Clock::now() - lastUpdateTime >= std::chrono::seconds(30)) {
                task->setState(new TimedOutState());
                return;
            }
        }
    }
}

void TimeoutMonitor::update() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        lastUpdateTime = Clock::now();
    }

    cv.notify_one(); 
}

void TimeoutMonitor::finished() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (stopRequested) return;
        stopRequested = true;
    }
    cv.notify_one();

    if (worker_thread.joinable()) {
        worker_thread.join();
    }
}

TimeoutMonitor::~TimeoutMonitor() {
    finished();
}