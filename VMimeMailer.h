#ifndef VMIMEMAILER_H
#define VMIMEMAILER_H

#include <string>

class VMimeMailer
{
private:
    std::string smtpHost;
    int smtpPort;
    bool connected;
    std::string lastMimeSent;

public:
    VMimeMailer(const std::string &host = "smtp.tividy.org", int port = 587);
    virtual ~VMimeMailer();
    virtual bool connectSMTP(std::string host, int port);
    virtual bool sendMimeMessage(std::string from, std::string to, std::string subject, std::string body);
    std::string getSmtpHost() const;
    int getSmtpPort() const;
    std::string getLastMimeSent() const;
};

#endif