#ifndef TIMEOUTOBSERVER_H
#define TIMEOUTOBSERVER_H

#include "Observer.h"
#include <string>

class Leaf;
class Status;
class Notifier;

class TimeoutObserver : public Observer {

private:
	Leaf* subject;
	Status* observerState;
	Notifier* notifier;
	std::string contact;

public:
	void update();
};

#endif
