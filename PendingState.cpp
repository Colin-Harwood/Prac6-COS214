// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#include "PendingState.h"
#include "CancelledState.h"
#include "InProgressState.h"

TaskState* PendingState::start(Task*) { return new InProgressState(); }
TaskState* PendingState::cancel(Task*) { return new CancelledState(); }

std::string PendingState::getName() const { return "Pending"; }
TaskState* PendingState::clone() const { return new PendingState(*this); }
