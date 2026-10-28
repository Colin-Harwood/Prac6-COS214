#include "TaskGroup.h"
#include "AllPriorityIterator.h"
#include "MediumPriorityIterator.h"
#include "HighPriorityIterator.h"

void TaskGroup::add(std::shared_ptr<Task> t) {
	// TODO - implement TaskGroup::add
	throw "Not yet implemented";
}

void TaskGroup::remove(const Task* t) {
	// TODO - implement TaskGroup::remove
	throw "Not yet implemented";
}

void TaskGroup::execute() {
	// TODO - implement TaskGroup::execute
	throw "Not yet implemented";
}

void TaskGroup::undo() {
	// TODO - implement TaskGroup::undo
	throw "Not yet implemented";
}

std::string TaskGroup::name() {
	// TODO - implement TaskGroup::name
	throw "Not yet implemented";
}

int TaskGroup::totalTime() {
	// TODO - implement TaskGroup::totalTime
	throw "Not yet implemented";
}

Iterator* TaskGroup::createAllIterator() {
	// TODO - implement TaskGroup::createAllIterator
	throw "Not yet implemented";
}

Iterator* TaskGroup::createMedIterator() {
	// TODO - implement TaskGroup::createMedIterator
	throw "Not yet implemented";
}

Iterator* TaskGroup::createRushIterator() {
	// TODO - implement TaskGroup::createRushIterator
	throw "Not yet implemented";
}
