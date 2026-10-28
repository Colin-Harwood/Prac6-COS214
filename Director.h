#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <string>

class Builder;
class PriorityStrategy;

class Director {

private:
	Builder* builder;

public:
	void construct(std::string name, std::string user, int timer, PriorityStrategy* p);
};

#endif
