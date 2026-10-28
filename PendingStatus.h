#ifndef PENDINGSTATUS_H
#define PENDINGSTATUS_H

#include "Status.h"

class PendingStatus : public Status {

public:
	void handle(Task* t);

	std::string getName();
};

#endif
