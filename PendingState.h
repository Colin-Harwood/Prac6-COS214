#ifndef PENDINGSTATE_H
#define PENDINGSTATE_H

#include "TaskState.h"

class PendingState : public TaskState {
public:
    TaskState* start(Task* t) override;
    TaskState* cancel(Task* t) override;
    std::string getName() const override;
    TaskState* clone() const override;
};

#endif
