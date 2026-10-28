#include "TividyFacade.h"
#include "Task.h"
#include "Leaf.h"
#include "TaskGroup.h"
#include "TaskCommand.h"
#include "ConditionalTaskCommand.h"
#include "TimeoutObserver.h"
#include "EmailNotifierAdapter.h"
#include "LowPriorityStrategy.h"
#include "MediumPriorityStrategy.h"
#include "HighPriorityStrategy.h"
#include "Iterator.h"

std::string TividyFacade::createTask(std::string name, std::string user, int timer, std::string level) {
	// TODO - implement TividyFacade::createTask
	throw "Not yet implemented";
}

std::string TividyFacade::createGroup(std::string name) {
	// TODO - implement TividyFacade::createGroup
	throw "Not yet implemented";
}

void TividyFacade::addToGroup(std::string groupId, std::string taskId) {
	// TODO - implement TividyFacade::addToGroup
	throw "Not yet implemented";
}

void TividyFacade::addToWorkflow(std::string taskId) {
	// TODO - implement TividyFacade::addToWorkflow
	throw "Not yet implemented";
}

void TividyFacade::setPriority(std::string taskId, std::string level) {
	// TODO - implement TividyFacade::setPriority
	throw "Not yet implemented";
}

void TividyFacade::setBranches(std::string taskId, std::string okId, std::string failId) {
	// TODO - implement TividyFacade::setBranches
	throw "Not yet implemented";
}

void TividyFacade::watchTimeout(std::string taskId, std::string contact) {
	// TODO - implement TividyFacade::watchTimeout
	throw "Not yet implemented";
}

void TividyFacade::startWorkflow() {
	// TODO - implement TividyFacade::startWorkflow
	throw "Not yet implemented";
}

void TividyFacade::advanceClock(int minutes) {
	// TODO - implement TividyFacade::advanceClock
	throw "Not yet implemented";
}

void TividyFacade::undoTask(std::string taskId) {
	// TODO - implement TividyFacade::undoTask
	throw "Not yet implemented";
}

void TividyFacade::cancelTask(std::string taskId) {
	// TODO - implement TividyFacade::cancelTask
	throw "Not yet implemented";
}

void TividyFacade::escalateTask(std::string taskId) {
	// TODO - implement TividyFacade::escalateTask
	throw "Not yet implemented";
}

std::vector<std::shared_ptr<Task>> TividyFacade::listTasks(std::string level) {
	// TODO - implement TividyFacade::listTasks
	throw "Not yet implemented";
}

std::string TividyFacade::statusReport() {
	// TODO - implement TividyFacade::statusReport
	throw "Not yet implemented";
}

std::shared_ptr<Task> TividyFacade::find(std::string taskId) {
	// TODO - implement TividyFacade::find
	throw "Not yet implemented";
}
