#ifndef STATUS_H
#define STATUS_H

#include <string>

class Task;

class Status {

public:
	virtual ~Status() {}

	virtual void handle(Task* t) = 0;

	virtual std::string getName() = 0;
};

#endif
