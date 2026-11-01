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
    std::string buildTaskSubject(Task *task);
    std::string buildTaskBody(Task *task);

public:
    VMimeEmailAdapter(VMimeMailer *mailer,
                      std::string sender = "litekd18@gmail.com",
                      const std::string &defaultRecipient = "");
    ~VMimeEmailAdapter() override;

    using Target::notifyProjectComplete;
    using Target::notifyStateChange;
    using Target::notifyTaskComplete;

    void notifyTaskComplete(Task *task, std::string recipientEmail) override;
    void notifyProjectComplete(Task *project, std::string auditLog, std::string recipientEmail) override;
    void notifyStateChange(Task *task, const std::string &eventAction, const std::string &recipientEmail) override;
};

#endif