#include "InProgressState.h"
#include "CancelledState.h"
#include "CompletedState.h"
#include "RejectedState.h"
#include "TimedOutState.h"

TaskState* InProgressState::complete(Task*) { return new CompletedState(); }
TaskState* InProgressState::reject(Task*) { return new RejectedState(); }
TaskState* InProgressState::timeout(Task*) { return new TimedOutState(); }
TaskState* InProgressState::cancel(Task*) { return new CancelledState(); }

std::string InProgressState::getName() const { return "InProgress"; }
TaskState* InProgressState::clone() const { return new InProgressState(*this); }
