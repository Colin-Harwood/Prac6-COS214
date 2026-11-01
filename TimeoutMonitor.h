/**
 * @file TimeoutMonitor.h
 * @author Colin Harwood
 * @brief Uses threads to check if the state of the task has changed in the last 30 seconds, and if it hasn't set the task to TimedOut
 * 
 * 
 */

#ifndef TIMEOUT_MONITOR
#define TIMEOUT_MONITOR

#include "Observer.h"
#include "Task.h"
#include "TimedOutState.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

class TimeoutMonitor : public Observer{
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
    /**
     * @brief Takes in and stores the task, and then creates the thread
     */
    TimeoutMonitor(Task* task);

    /**
     * @brief Safely cleans up thread and memory so no leak
     */
    ~TimeoutMonitor();

    /**
     * @brief Updates time to reset thread timeout counter
     */
    void update();

    /**
     * @brief Stop counter so thread stops checking for timeout and return
     */
    void finished();
};


#endif