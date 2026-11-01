#ifndef COMPLETEDSTATE_H
#define COMPLETEDSTATE_H

#include "TaskState.h"

class CompletedState : public TaskState {
public:
    TaskState* reopen(Task* t) override;
    std::string getName() const override;
    bool isFinished() const override;
    TaskState* clone() const override;
};

#endif
