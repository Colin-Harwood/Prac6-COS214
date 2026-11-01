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

    /**
     * @brief Recursively outputs chained vmime::exception details to std::cerr.
     * @param e The caught VMime exception.
     */
    void printVMimeException(const vmime::exception& e) const; // for debugging 

public:
    /**
     * @brief Constructs a VMimeMailer configured for an external SMTP server.
     * @param host SMTP server hostname (defaults to "smtp.gmail.com")
     * @param port SMTP server port (defaults to 587)
     */
    VMimeMailer(const std::string &host = "smtp.gmail.com", int port = 587);

    /**
     * @brief Virtual destructor
     */
    virtual ~VMimeMailer();

    /**
     * @brief Sets the SMTP authentication username and App Password.
     * @param username Full Gmail address used for SMTP authentication
     * @param appPassword 16-character Google App Password
     */
    void setCredentials(const std::string& username, const std::string& appPassword); // maybe???

    /**
     * @brief Initializes the VMime session, STARTTLS properties, and certificate verifier.
     * @param host Target SMTP server hostname
     * @param port Target SMTP server port
     * @return true if the transport service was configured without error, false otherwise
     */
    virtual bool connectSMTP(std::string host, int port);

    /**
     * @brief Builds an RFC-2822 MIME message and transmits it over the authenticated SMTP transport.
     * @param from Sender email address
     * @param to Recipient email address
     * @param subject Email Subject header text
     * @param body Plain-text UTF-8 email body
     * @return true if the email was built and transmitted over SMTP, false on failure
     */
    virtual bool sendMimeMessage(std::string from, std::string to, std::string subject, std::string body);

    /**
     * @brief Gets the configured SMTP server hostname.
     * @return The SMTP host string
     */
    std::string getSmtpHost() const;

    /**
     * @brief Gets the configured SMTP server port.
     * @return The SMTP port number
     */
    int getSmtpPort() const;

    /**
     * @brief Gets the raw RFC-2822 MIME payload generated during the last sendMimeMessage() call.
     * @return The serialized MIME message string
     */
    std::string getLastGeneratedMime() const;
};

#endif