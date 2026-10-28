#ifndef CANCELLEDSTATUS_H
#define CANCELLEDSTATUS_H

#include "Status.h"

class CancelledStatus : public Status {

public:
	void handle(Task* t);

	std::string getName();
};

#endif
