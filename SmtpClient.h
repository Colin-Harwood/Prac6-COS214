#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <string>

class SmtpClient {

public:
	bool connect(std::string host, int port);

	int transmit(std::string from, std::string to, std::string body);

	void disconnect();
};

#endif
