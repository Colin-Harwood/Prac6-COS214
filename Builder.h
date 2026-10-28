#ifndef BUILDER_H
#define BUILDER_H

#include <string>

class PriorityStrategy;

class Builder {

public:
	virtual ~Builder() {}

	virtual void buildTaskId() = 0;

	virtual void buildStatus() = 0;

	virtual void buildTimer(int t) = 0;

	virtual void buildUser(std::string u) = 0;

	virtual void buildPriority(PriorityStrategy* p) = 0;
};

#endif
