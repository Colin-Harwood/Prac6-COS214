#ifndef TIMEDOUTSTATUS_H
#define TIMEDOUTSTATUS_H

#include "Status.h"

class TimedOutStatus : public Status {

public:
	void handle(Task* t);

	std::string getName();
};

#endif
