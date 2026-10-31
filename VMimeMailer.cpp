
#include "VMimeMailer.h"
#include <iostream>
#include <sstream>

namespace
{
    class TividyCertVerifier final
        : public vmime::security::cert::certificateVerifier
    {
    public:
        virtual void verify(
            const vmime::shared_ptr<vmime::security::cert::certificateChain> &chain,
            const vmime::string &hostname)
        {
            (void)chain;
            (void)hostname;
        }
        
        virtual void verify(
            vmime::shared_ptr<vmime::security::cert::certificateChain> chain,
            const vmime::string &hostname) override
        {
            (void)chain;
            (void)hostname;
        }
    };
}

VMimeMailer::VMimeMailer(const std::string &host, int port)
    : smtpHost(host),
      smtpPort(port),
      authUsername(""),
      authPassword(""),
      session(vmime::net::session::create()),
      transport(nullptr),
      lastGeneratedMime("")
{
}

VMimeMailer::~VMimeMailer()
{
}

void VMimeMailer::setCredentials(const std::string &username, const std::string &appPassword)
{
    this->authUsername = username;
    this->authPassword = appPassword;
}

void VMimeMailer::printVMimeException(const vmime::exception &e) const
{
    std::cerr << "* vmime::exceptions::" << e.name() << "\n"
              << "  what = " << e.what() << std::endl;
    if (e.other() != NULL)
    {
        printVMimeException(*e.other());
    }
}

bool VMimeMailer::connectSMTP(std::string host, int port)
{
    if (host.empty() || port <= 0)
    {
        return false;
    }

    this->smtpHost = host;
    this->smtpPort = port;

    try
    {
        session = vmime::net::session::create();

        vmime::utility::url url("smtp://" + smtpHost);
        transport = session->getTransport(url);

        transport->setProperty("server.address", smtpHost);
        transport->setProperty("server.port", smtpPort);

        transport->setProperty("connection.tls", true);
        transport->setProperty("connection.tls.required", true);

        if (!authUsername.empty())
        {
            transport->setProperty("options.need-authentication", true);
            transport->setProperty("auth.username", authUsername);
            transport->setProperty("auth.password", authPassword);
        }

        transport->setCertificateVerifier(vmime::make_shared<TividyCertVerifier>());

        return true;
    }
    catch (const vmime::exception &e)
    {
        printVMimeException(e);
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "std::exception in connectSMTP: " << e.what() << std::endl;
        return false;
    }
}

bool VMimeMailer::sendMimeMessage(std::string from, std::string to, std::string subject, std::string body)
{
    if (to.empty() || !transport)
    {
        return false;
    }

    std::string effectiveFrom = authUsername.empty() ? from : authUsername;
    if (effectiveFrom.empty())
    {
        return false;
    }

    try
    {
        vmime::messageBuilder mb;
        mb.setSubject(vmime::text(subject));
        mb.setExpeditor(vmime::mailbox(effectiveFrom));
        mb.getRecipients().appendAddress(vmime::make_shared<vmime::mailbox>(to));

        mb.getTextPart()->setCharset(vmime::charsets::UTF_8);
        mb.getTextPart()->setText(
            vmime::make_shared<vmime::stringContentHandler>(body));

        vmime::shared_ptr<vmime::message> msg = mb.construct();

        vmime::string rawMime;
        vmime::utility::outputStreamStringAdapter outStr(rawMime);
        msg->generate(outStr);
        lastGeneratedMime = rawMime;

        transport->connect();
        transport->send(msg);
        transport->disconnect();

        std::cout << "[VMime] Live email successfully sent to " << to << std::endl;
        return true;
    }
    catch (const vmime::exception &e)
    {
        printVMimeException(e);
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "std::exception in sendMimeMessage: " << e.what() << std::endl;
        return false;
    }
}

std::string VMimeMailer::getSmtpHost() const
{
    return smtpHost;
}

int VMimeMailer::getSmtpPort() const
{
    return smtpPort;
}

std::string VMimeMailer::getLastGeneratedMime() const
{
    return lastGeneratedMime;
}