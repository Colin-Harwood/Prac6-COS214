#ifndef MEDIUMPRIORITYSTRATEGY_H
#define MEDIUMPRIORITYSTRATEGY_H

#include "PriorityStrategy.h"

class MediumPriorityStrategy : public PriorityStrategy {

public:
	void route(Task* t);

	void escalate(Task* t);

	std::string getPriorityLevel();
};

#endif
