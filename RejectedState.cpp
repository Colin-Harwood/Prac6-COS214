#include "RejectedState.h"
#include "PendingState.h"

TaskState* RejectedState::reopen(Task*) { return new PendingState(); }

std::string RejectedState::getName() const { return "Rejected"; }
TaskState* RejectedState::clone() const { return new RejectedState(*this); }
