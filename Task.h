#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>
#include <vector>

class Command;
class Status;
class PriorityStrategy;
class Observer;
class Snapshot;
class Iterator;

class Task {

protected:
	std::shared_ptr<Task> next;
	std::string taskName;	// UML: -name. Renamed: C++ forbids a data member and member function both called name

private:
	std::unique_ptr<Command> command;
	std::string id;
	Status* status;
	int timer;
	std::string user;
	PriorityStrategy* priority;
	std::vector<Observer*> observerList;

public:
	virtual ~Task();

	void setNext(std::shared_ptr<Task> next);

	void handle();

	virtual void execute();

	virtual void undo();

	virtual void add(std::shared_ptr<Task> t);

	virtual void remove(const Task* t);

	virtual std::string name() = 0;

	virtual int totalTime() = 0;

	std::string getId();

	void advanceStatus();

	void setStatus(Status* s);

	Status* getStatus();

	void cancel();

	void setCommand(Command* c);

	void assignTo(std::string user);

	int getTimer();

	void tick(int minutes);

	void setPriority(PriorityStrategy* p);

	void route();

	void escalate();

	std::string getPriorityLevel();

	void attach(Observer* o);

	void detach(Observer* o);

	void notify();

	Snapshot* createSnapshot();

	void setSnapshot(Snapshot* s);

	virtual Iterator* createAllIterator() = 0;

	virtual Iterator* createMedIterator() = 0;

	virtual Iterator* createRushIterator() = 0;
};

#endif
