#ifndef COMMAND_H
#define COMMAND_H

#include <stack>
#include <string>

class Task;
class Snapshot;

class Command {

private:
	std::string name;
	std::stack<Snapshot*> snapshots;

protected:
	Task* receiver;

public:
	virtual ~Command();

	virtual void execute() = 0;

	virtual void undo() = 0;

	virtual void print() = 0;

	std::string getState();

private:
	void writeToLogFile();

protected:
	void storeSnapshot(Snapshot* s);

	Snapshot* retrieveSnapshot();
};

#endif
