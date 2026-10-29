// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#include "CompletedState.h"
#include "PendingState.h"

TaskState* CompletedState::reopen(Task*) { return new PendingState(); }

std::string CompletedState::getName() const { return "Completed"; }
bool CompletedState::isFinished() const { return true; }
TaskState* CompletedState::clone() const { return new CompletedState(*this); }
