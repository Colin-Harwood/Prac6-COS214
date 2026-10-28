#ifndef FAILEDSTATUS_H
#define FAILEDSTATUS_H

#include "Status.h"

class FailedStatus : public Status {

public:
	void handle(Task* t);

	std::string getName();
};

#endif
