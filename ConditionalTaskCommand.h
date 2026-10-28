#ifndef CONDITIONALTASKCOMMAND_H
#define CONDITIONALTASKCOMMAND_H

#include "Command.h"
#include <memory>

class Task;

class ConditionalTaskCommand : public Command {

private:
	bool condition;
	std::shared_ptr<Task> successBranch;
	std::shared_ptr<Task> failureBranch;

public:
	void execute();

	void undo();

	void print();
};

#endif
