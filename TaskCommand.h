#ifndef TASKCOMMAND_H
#define TASKCOMMAND_H

#include "Command.h"

class TaskCommand : public Command {

public:
	void execute();

	void undo();

	void print();
};

#endif
