#ifndef TIMEDOUTSTATE_H
#define TIMEDOUTSTATE_H

#include "TaskState.h"

class TimedOutState : public TaskState {
public:
    TaskState* start(Task* t) override;
    TaskState* cancel(Task* t) override;
    std::string getName() const override;
    TaskState* clone() const override;
};

#endif
