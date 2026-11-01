
#ifndef TARGET_H
#define TARGET_H

#include <string>

class Task;

class Target
{
protected:
    std::string recipientEmail;

public:
    explicit Target(const std::string &defaultRecipient = "");
    virtual ~Target();
    void setRecipientEmail(const std::string &email);
    std::string getRecipientEmail() const;
    virtual void notifyTaskComplete(Task *task, std::string recipientEmail) = 0;
    virtual void notifyProjectComplete(Task *project, std::string auditLog, std::string recipientEmail) = 0;
    virtual void notifyStateChange(Task *task, const std::string &eventAction, const std::string &recipientEmail) = 0;
    
    void notifyTaskComplete(Task *task);
    void notifyProjectComplete(Task *project, const std::string &auditLog);
    void notifyStateChange(Task *task, const std::string &eventAction);
};

#endif