#ifndef VMIMEMAILER_H
#define VMIMEMAILER_H

#include <string>
#include <vmime/vmime.hpp>

class VMimeMailer
{
private:
    std::string smtpHost;
    int smtpPort;
    std::string authUsername;
    std::string authPassword;
    vmime::shared_ptr<vmime::net::session> session; // VMime messaging session
    vmime::shared_ptr<vmime::net::transport> transport; // VMime SMTP transporter
    std::string lastGeneratedMime;

    void printVMimeException(const vmime::exception& e) const; // for debugging 

public:
    VMimeMailer(const std::string &host = "smtp.gmail.com", int port = 587);
    virtual ~VMimeMailer();

    void setCredentials(const std::string& username, const std::string& appPassword); // maybe???

    virtual bool connectSMTP(std::string host, int port);
    virtual bool sendMimeMessage(std::string from, std::string to, std::string subject, std::string body);
    std::string getSmtpHost() const;
    int getSmtpPort() const;
    std::string getLastGeneratedMime() const;
};

#endif