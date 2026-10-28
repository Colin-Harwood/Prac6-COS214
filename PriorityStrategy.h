#ifndef PRIORITYSTRATEGY_H
#define PRIORITYSTRATEGY_H

#include <string>

class Task;

class PriorityStrategy {

public:
	virtual ~PriorityStrategy() {}

	virtual void route(Task* t) = 0;

	virtual void escalate(Task* t) = 0;

	virtual std::string getPriorityLevel() = 0;
};

#endif
