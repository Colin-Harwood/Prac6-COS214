#ifndef TIMEOUT_MONITOR
#define TIMEOUT_MONITOR

#include "Observer.h"
#include "Task.h"

#include <chrono>

class TimeoutMonitor : public Observer
{
private:
    Task* task;
    TaskState* state;
    using Clock = std::chrono::steady_clock;
    Clock::time_point start_time;
public:
    TimeoutMonitor(Task* task);
    ~TimeoutMonitor();
    void update(TaskState* state);
};


#endif