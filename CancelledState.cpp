// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#include "CancelledState.h"


std::string CancelledState::getName() const { return "Cancelled"; }
bool CancelledState::isFinished() const { return true; }
TaskState* CancelledState::clone() const { return new CancelledState(*this); }
