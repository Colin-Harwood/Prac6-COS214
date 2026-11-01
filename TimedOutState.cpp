#include "TimedOutState.h"
#include "CancelledState.h"
#include "InProgressState.h"

TaskState* TimedOutState::start(Task*) { return new InProgressState(); }
TaskState* TimedOutState::cancel(Task*) { return new CancelledState(); }

std::string TimedOutState::getName() const { return "TimedOut"; }
TaskState* TimedOutState::clone() const { return new TimedOutState(*this); }
