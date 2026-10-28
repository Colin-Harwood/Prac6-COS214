#ifndef MEDIUMPRIORITYITERATOR_H
#define MEDIUMPRIORITYITERATOR_H

#include "Iterator.h"
#include <cstddef>
#include <vector>

class MediumPriorityIterator : public Iterator {

private:
	std::vector<Task*> items;
	std::size_t current;

public:
	void first();

	void next();

	bool isDone();

	Task* currentItem();
};

#endif
