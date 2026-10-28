#ifndef TASKBUILDER_H
#define TASKBUILDER_H

#include "Builder.h"
#include <memory>

class Task;
class Leaf;

class TaskBuilder : public Builder {

private:
	std::shared_ptr<Leaf> task;

public:
	std::shared_ptr<Task> getResult();

	void buildTaskId();

	void buildStatus();

	void buildTimer(int t);

	void buildUser(std::string u);

	void buildPriority(PriorityStrategy* p);
};

#endif
