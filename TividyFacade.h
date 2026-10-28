#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H

#include "Workflow.h"
#include "Director.h"
#include "TaskBuilder.h"
#include "Observer.h"
#include "PriorityStrategy.h"
#include "Notifier.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Task;

class TividyFacade {

private:
	Workflow engine;
	Director director;
	TaskBuilder builder;
	std::map<std::string, std::shared_ptr<Task>> tasks;
	std::vector<std::unique_ptr<Observer>> observers;
	std::map<std::string, std::unique_ptr<PriorityStrategy>> strategies;
	std::unique_ptr<Notifier> notifier;

public:
	std::string createTask(std::string name, std::string user, int timer, std::string level);

	std::string createGroup(std::string name);

	void addToGroup(std::string groupId, std::string taskId);

	void addToWorkflow(std::string taskId);

	void setPriority(std::string taskId, std::string level);

	void setBranches(std::string taskId, std::string okId, std::string failId);

	void watchTimeout(std::string taskId, std::string contact);

	void startWorkflow();

	void advanceClock(int minutes);

	void undoTask(std::string taskId);

	void cancelTask(std::string taskId);

	void escalateTask(std::string taskId);

	std::vector<std::shared_ptr<Task>> listTasks(std::string level);

	std::string statusReport();

private:
	std::shared_ptr<Task> find(std::string taskId);
};

#endif
