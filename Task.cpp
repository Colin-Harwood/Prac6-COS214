#include "Task.h"
#include "Command.h"
#include "Status.h"
#include "PriorityStrategy.h"
#include "Observer.h"
#include "Snapshot.h"
#include "Iterator.h"

Task::~Task() {
	// Defined here, not in the header, so std::unique_ptr<Command> sees the complete Command type
}

void Task::setNext(std::shared_ptr<Task> next) {
	// TODO - implement Task::setNext
	throw "Not yet implemented";
}

void Task::handle() {
	// TODO - implement Task::handle
	throw "Not yet implemented";
}

void Task::execute() {
	// TODO - implement Task::execute
	throw "Not yet implemented";
}

void Task::undo() {
	// TODO - implement Task::undo
	throw "Not yet implemented";
}

void Task::add(std::shared_ptr<Task> t) {
	// TODO - implement Task::add
	throw "Not yet implemented";
}

void Task::remove(const Task* t) {
	// TODO - implement Task::remove
	throw "Not yet implemented";
}

std::string Task::getId() {
	// TODO - implement Task::getId
	throw "Not yet implemented";
}

void Task::advanceStatus() {
	// TODO - implement Task::advanceStatus
	throw "Not yet implemented";
}

void Task::setStatus(Status* s) {
	// TODO - implement Task::setStatus
	throw "Not yet implemented";
}

Status* Task::getStatus() {
	// TODO - implement Task::getStatus
	throw "Not yet implemented";
}

void Task::cancel() {
	// TODO - implement Task::cancel
	throw "Not yet implemented";
}

void Task::setCommand(Command* c) {
	// TODO - implement Task::setCommand
	throw "Not yet implemented";
}

void Task::assignTo(std::string user) {
	// TODO - implement Task::assignTo
	throw "Not yet implemented";
}

int Task::getTimer() {
	// TODO - implement Task::getTimer
	throw "Not yet implemented";
}

void Task::tick(int minutes) {
	// TODO - implement Task::tick
	throw "Not yet implemented";
}

void Task::setPriority(PriorityStrategy* p) {
	// TODO - implement Task::setPriority
	throw "Not yet implemented";
}

void Task::route() {
	// TODO - implement Task::route
	throw "Not yet implemented";
}

void Task::escalate() {
	// TODO - implement Task::escalate
	throw "Not yet implemented";
}

std::string Task::getPriorityLevel() {
	// TODO - implement Task::getPriorityLevel
	throw "Not yet implemented";
}

void Task::attach(Observer* o) {
	// TODO - implement Task::attach
	throw "Not yet implemented";
}

void Task::detach(Observer* o) {
	// TODO - implement Task::detach
	throw "Not yet implemented";
}

void Task::notify() {
	// TODO - implement Task::notify
	throw "Not yet implemented";
}

Snapshot* Task::createSnapshot() {
	// TODO - implement Task::createSnapshot
	throw "Not yet implemented";
}

void Task::setSnapshot(Snapshot* s) {
	// TODO - implement Task::setSnapshot
	throw "Not yet implemented";
}
