// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#include "TaskState.h"

InvalidTransition::InvalidTransition(const std::string& stateName, const std::string& action)
    : std::logic_error("Cannot " + action + " a task that is " + stateName) {}

TaskState::~TaskState() {}

// Default: the move is not allowed from this state
TaskState* TaskState::start(Task*)    { throw InvalidTransition(getName(), "start"); }
TaskState* TaskState::complete(Task*) { throw InvalidTransition(getName(), "complete"); }
TaskState* TaskState::reject(Task*)   { throw InvalidTransition(getName(), "reject"); }
TaskState* TaskState::timeout(Task*)  { throw InvalidTransition(getName(), "time out"); }
TaskState* TaskState::cancel(Task*)   { throw InvalidTransition(getName(), "cancel"); }
TaskState* TaskState::reopen(Task*)   { throw InvalidTransition(getName(), "reopen"); }

bool TaskState::isFinished() const { return false; }
