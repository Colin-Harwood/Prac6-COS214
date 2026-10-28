#ifndef LEAF_H
#define LEAF_H

#include "Task.h"

class Leaf : public Task {

private:
	int duration;

public:
	void execute();

	void undo();

	std::string name();

	int totalTime();

	Iterator* createAllIterator();

	Iterator* createMedIterator();

	Iterator* createRushIterator();
};

#endif
