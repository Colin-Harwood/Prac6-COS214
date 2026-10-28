#ifndef LOWPRIORITYSTRATEGY_H
#define LOWPRIORITYSTRATEGY_H

#include "PriorityStrategy.h"

class LowPriorityStrategy : public PriorityStrategy {

public:
	void route(Task* t);

	void escalate(Task* t);

	std::string getPriorityLevel();
};

#endif
