#ifndef WORKFLOW_H
#define WORKFLOW_H

#include <memory>

class Task;

class Workflow {

private:
	std::shared_ptr<Task> head;
	std::shared_ptr<Task> tail;

public:
	void addTask(std::shared_ptr<Task> t);

	void run();

	void tick(int minutes);
};

#endif
