#ifndef TASKGROUP_H
#define TASKGROUP_H

#include "Task.h"

class TaskGroup : public Task {

private:
	std::vector<std::shared_ptr<Task>> children;

public:
	void add(std::shared_ptr<Task> t);

	void remove(const Task* t);

	void execute();

	void undo();

	std::string name();

	int totalTime();

	Iterator* createAllIterator();

	Iterator* createMedIterator();

	Iterator* createRushIterator();
};

#endif
