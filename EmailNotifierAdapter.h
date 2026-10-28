#ifndef EMAILNOTIFIERADAPTER_H
#define EMAILNOTIFIERADAPTER_H

#include "Notifier.h"

class SmtpClient;

class EmailNotifierAdapter : public Notifier {

private:
	SmtpClient* client;

public:
	void send(std::string to, std::string message);
};

#endif
