#ifndef HIGHPRIORITYSTRATEGY_H
#define HIGHPRIORITYSTRATEGY_H

#include "PriorityStrategy.h"

class HighPriorityStrategy : public PriorityStrategy {

public:
	void route(Task* t);

	void escalate(Task* t);

	std::string getPriorityLevel();
};

#endif
