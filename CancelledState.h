#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H

#include "TaskState.h"

class CancelledState : public TaskState {
public:
    std::string getName() const override;
    bool isFinished() const override;
    TaskState* clone() const override;
};

#endif
