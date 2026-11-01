#ifndef VMIMEEMAILADAPTER_H
#define VMIMEEMAILADAPTER_H

#include <string>
#include "Target.h"

class VMimeMailer;
class Task;

class VMimeEmailAdapter : public Target
{
private:
    VMimeMailer *adaptee;
    std::string senderAddress;

    /**
     * @brief Builds a formatted email subject line from a Task's metadata.
     * @param task Pointer to the Task being reporteds.
     * @return Formatted subject line string.
     */
    std::string buildTaskSubject(Task *task);

    /**
     * @brief Builds a formatted plain-text email body summarizing a Task's state and deadline.
     * @param task Pointer to the Task being reported.
     * @return Formatted multi-line email body string.
     */
    std::string buildTaskBody(Task *task);

public:
    /**
     * @brief Constructs a VMimeEmailAdapter wrapping a VMimeMailer instance.
     * @param mailer Pointer to the VMimeMailer adaptee.
     * @param sender System sender email address (defaults to "litekd18@gmail.com")
     * @param defaultRecipient Optional initial recipient email address
     */
    VMimeEmailAdapter(VMimeMailer *mailer,
                      std::string sender = "litekd18@gmail.com",
                      const std::string &defaultRecipient = "");

    /**
     * @brief Destructor 
     */
    ~VMimeEmailAdapter() override;

    using Target::notifyProjectComplete;
    using Target::notifyStateChange;
    using Target::notifyTaskComplete;

    /**
     * @brief Translates a completed Task into a MIME email sent via VMimeMailer.
     * @param task Pointer to the completed Task.
     * @param recipientEmail Destination email address.
     */
    void notifyTaskComplete(Task *task, std::string recipientEmail) override;

    /**
     * @brief Translates a completed project and its audit log into a summary email sent via VMimeMailer.
     * @param project Pointer to the root project Task.
     * @param auditLog Complete workflow history from LogQueue.
     * @param recipientEmail Destination email address.
     */
    void notifyProjectComplete(Task *project, std::string auditLog, std::string recipientEmail) override;

    /**
     * @brief Translates a Task state-change event into an alert email sent via VMimeMailer.
     * @param task Pointer to the Task whose state changed.
     * @param eventAction Label describing the state transition.
     * @param recipientEmail Destination email address.
     */
    void notifyStateChange(Task *task, const std::string &eventAction, const std::string &recipientEmail) override;
};

#endif