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
                      std::string sender = "noreply@tividy.org",
                      const std::string &defaultRecipient = "");
    ~VMimeEmailAdapter() override;
    void notifyTaskComplete(Task *task, std::string recipientEmail) override;
    void notifyProjectComplete(Task *project, std::string auditLog, std::string recipientEmail) override;
    void notifyStateChange(Task *task, const std::string &eventAction, const std::string &recipientEmail) override;
};

#endif