#ifndef ITERATOR_H
#define ITERATOR_H

class Task;

class Iterator {

public:
	virtual ~Iterator() {}

	virtual void first() = 0;

	virtual void next() = 0;

	virtual bool isDone() = 0;

	virtual Task* currentItem() = 0;
};

#endif
