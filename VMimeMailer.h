/**
 * @file VMimeMailer.h
 * @brief Low-level SMTP client using VMime for email transmission.
 */

#ifndef VMIMEMAILER_H
#define VMIMEMAILER_H

#include <string>
#include <vmime/vmime.hpp>

/**
 * @class VMimeMailer
 * @brief Adaptee class encapsulating low-level VMime SMTP/STARTTLS and MIME operations.
 *
 * Manages VMime session and transport services, configures STARTTLS encryption
 * and custom X.509 certificate verification, authenticates via Google App Passwords,
 * and constructs RFC-2822 MIME messages using vmime::messageBuilder.
 */
class VMimeMailer
{
private:
    std::string smtpHost;                               /**< SMTP server hostname (e.g., "smtp.gmail.com") */
    int smtpPort;                                       /**< SMTP server port (e.g., 587) */
    std::string authUsername;                           /**< Authenticated SMTP sender email address */
    std::string authPassword;                           /**< 16-character Google App Password */
    vmime::shared_ptr<vmime::net::session> session;     /**< VMime messaging session */
    vmime::shared_ptr<vmime::net::transport> transport; /**< VMime SMTP transporter */
    std::string lastGeneratedMime;                      /**< Raw RFC-2822 MIME string from the last built message */

    /**
     * @brief Recursively prints chained vmime::exception details to std::cerr for debugging.
     * @param e Caught vmime::exception reference.
     */
    void printVMimeException(const vmime::exception& e) const;

public:
    /**
     * @brief Constructs a VMimeMailer pointing to the specified SMTP server and port.
     * @param host SMTP server hostname (defaults to "smtp.gmail.com").
     * @param port SMTP server port (defaults to 587).
     */
    VMimeMailer(const std::string &host = "smtp.gmail.com", int port = 587);

    /**
     * @brief Virtual destructor.
     */
    virtual ~VMimeMailer();

    /**
     * @brief Configures the Gmail sender address and 16-character App Password for SMTP authentication.
     * @param username Sender Gmail address.
     * @param appPassword 16-character Google App Password.
     */
    void setCredentials(const std::string& username, const std::string& appPassword);

    /**
     * @brief Initializes the VMime SMTP transport session, STARTTLS settings, and certificate verifier.
     * @param host SMTP server hostname.
     * @param port SMTP server port.
     * @return true if the transport service was configured successfully, false otherwise.
     */
    virtual bool connectSMTP(std::string host, int port);

    /**
     * @brief Builds an RFC-2822 MIME message and transmits it over the authenticated SMTP transport.
     * @param from Sender email address.
     * @param to Recipient email address.
     * @param subject Email subject header text.
     * @param body Plain-text UTF-8 email body.
     * @return true if the MIME message was built and transmitted successfully, false otherwise.
     */
    virtual bool sendMimeMessage(std::string from, std::string to, std::string subject, std::string body);

    /**
     * @brief Returns the configured SMTP server hostname.
     * @return SMTP host string.
     */
    std::string getSmtpHost() const;

    /**
     * @brief Returns the configured SMTP server port.
     * @return SMTP port number.
     */
    int getSmtpPort() const;

    /**
     * @brief Returns the raw RFC-2822 MIME message string generated during the most recent sendMimeMessage() call.
     * @return Generated MIME string.
     */
    std::string getLastGeneratedMime() const;
};

#endif
