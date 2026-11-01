
#ifndef VMIMEEMAILADAPTER_H
#define VMIMEEMAILADAPTER_H

#include <string>
#include "Target.h"

class VMimeMailer;
class Task;

/**
 * @class VMimeEmailAdapter
 * @brief Object Adapter translating domain-level Task notifications into VMimeMailer calls.
 *
 * Implements the Target interface and holds a pointer to a VMimeMailer Adaptee.
 * Extracts metadata from Task pointers (name, department level, deadline, and state)
 * and formats MIME-compatible subject and body strings for SMTP transmission.
 */
class VMimeEmailAdapter : public Target
{
private:
    VMimeMailer *adaptee;       /**< Pointer to the VMimeMailer Adaptee instance */
    std::string senderAddress;  /**< System sender email address used in the From header */

    /**
     * @brief Constructs a formatted email subject line from a Task's attributes.
     * @param task Pointer to the Task being reported.
     * @return Formatted email subject string.
     */
    std::string buildTaskSubject(Task *task);

    /**
     * @brief Constructs a formatted plain-text email body from a Task's attributes.
     * @param task Pointer to the Task being reported.
     * @return Formatted multi-line email body string.
     */
    std::string buildTaskBody(Task *task);

public:
    /**
     * @brief Constructs a VMimeEmailAdapter wrapping a VMimeMailer Adaptee.
     * @param mailer Pointer to the VMimeMailer instance to adapt.
     * @param sender System sender email address (defaults to "litekd18@gmail.com").
     * @param defaultRecipient Optional initial recipient email address.
     */
    VMimeEmailAdapter(VMimeMailer *mailer,
                      std::string sender = "litekd18@gmail.com",
                      const std::string &defaultRecipient = "");

    /**
     * @brief Destructor.
     */
    ~VMimeEmailAdapter() override;

    using Target::notifyProjectComplete;
    using Target::notifyStateChange;
    using Target::notifyTaskComplete;

    /**
     * @brief Translates a task completion event into an SMTP email via VMimeMailer.
     * @param task Pointer to the completed Task.
     * @param recipientEmail Destination email address.
     */
    void notifyTaskComplete(Task *task, std::string recipientEmail) override;

    /**
     * @brief Translates a project completion event and audit log into an SMTP email via VMimeMailer.
     * @param project Pointer to the root project Task.
     * @param auditLog Compiled workflow execution audit log.
     * @param recipientEmail Destination email address.
     */
    void notifyProjectComplete(Task *project, std::string auditLog, std::string recipientEmail) override;

    /**
     * @brief Translates a task state-change event into an alert email via VMimeMailer.
     * @param task Pointer to the Task whose state changed.
     * @param eventAction Description of the triggered state event.
     * @param recipientEmail Destination email address.
     */
    void notifyStateChange(Task *task, const std::string &eventAction, const std::string &recipientEmail) override;
};

#endif