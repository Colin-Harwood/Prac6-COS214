#ifndef INPROGRESSSTATUS_H
#define INPROGRESSSTATUS_H

#include "Status.h"

class InProgressStatus : public Status {

public:
	void handle(Task* t);

	std::string getName();
};

#endif
