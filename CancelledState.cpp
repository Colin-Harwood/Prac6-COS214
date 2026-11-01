/**
 * @file CancelledState.cpp
 * @brief Implements the moves CancelledState allows.
 */
#include "CancelledState.h"

std::string CancelledState::getName() const { return "Cancelled"; }
bool CancelledState::isFinished() const { return true; }
TaskState* CancelledState::clone() const { return new CancelledState(*this); }
