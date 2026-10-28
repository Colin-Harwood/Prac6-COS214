#ifndef COMPLETEDSTATUS_H
#define COMPLETEDSTATUS_H

#include "Status.h"

class CompletedStatus : public Status {

public:
	void handle(Task* t);

	std::string getName();
};

#endif
