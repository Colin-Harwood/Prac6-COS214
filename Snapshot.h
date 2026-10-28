#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <string>

class Status;
class PriorityStrategy;

class Snapshot {
	friend class Task;

private:
	std::string taskName;
	std::string id;
	Status* status;
	int timer;
	std::string user;
	PriorityStrategy* priority;

	Snapshot(std::string name, std::string id, std::string user, Status* s, int timer, PriorityStrategy* p);

public:
	std::string describe();
};

#endif
