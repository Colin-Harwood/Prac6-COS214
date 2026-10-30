#include "VMimeMailer.h"
#include <iostream>
#include <sstream>

VMimeMailer::VMimeMailer(const std::string &host, int port)
    : smtpHost(host), smtpPort(port), connected(false), lastMimeSent("")
{
}

VMimeMailer::~VMimeMailer()
{
}

bool VMimeMailer::connectSMTP(std::string host, int port)
{
    if (host.empty() || port <= 0)
    {
        connected = false;
        return false;
    }
    this->smtpHost = host;
    this->smtpPort = port;
    this->connected = true;
    return true;
}

bool VMimeMailer::sendMimeMessage(std::string from, std::string to, std::string subject, std::string body)
{
    if (!connected || from.empty() || to.empty())
    {
        return false;
    }

    std::ostringstream mimeStream;
    mimeStream << "=== [VMime SMTP Transmission via " << smtpHost << ":" << smtpPort << "] ===\n"
               << "From: " << from << "\n"
               << "To: " << to << "\n"
               << "Subject: " << subject << "\n"
               << "------------------------------------------------------------\n"
               << body << "\n"
               << "============================================================\n";

    lastMimeSent = mimeStream.str();
    std::cout << lastMimeSent;
    return true;
}

std::string VMimeMailer::getSmtpHost() const
{
    return smtpHost;
}

int VMimeMailer::getSmtpPort() const
{
    return smtpPort;
}

std::string VMimeMailer::getLastMimeSent() const
{
    return lastMimeSent;
}