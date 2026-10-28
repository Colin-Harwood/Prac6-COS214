#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <string>

class Notifier {

public:
	virtual ~Notifier() {}

	virtual void send(std::string to, std::string message) = 0;
};

#endif
