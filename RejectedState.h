#ifndef REJECTEDSTATE_H
#define REJECTEDSTATE_H

#include "TaskState.h"

class RejectedState : public TaskState {
public:
    TaskState* reopen(Task* t) override;
    std::string getName() const override;
    TaskState* clone() const override;
};

#endif
