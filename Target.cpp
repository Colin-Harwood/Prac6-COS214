#include "Target.h"

Target::Target(const std::string &defaultRecipient)
    : recipientEmail(defaultRecipient)
{
}

Target::~Target()
{
}

void Target::setRecipientEmail(const std::string &email)
{
    this->recipientEmail = email;
}

std::string Target::getRecipientEmail() const
{
    return this->recipientEmail;
}

void Target::notifyTaskComplete(Task *task)
{
    if (!recipientEmail.empty())
    {
        notifyTaskComplete(task, recipientEmail);
    }
}

void Target::notifyProjectComplete(Task *project, const std::string &auditLog)
{
    if (!recipientEmail.empty())
    {
        notifyProjectComplete(project, auditLog, recipientEmail);
    }
}

void Target::notifyStateChange(Task *task, const std::string &eventAction)
{
    if (!recipientEmail.empty())
    {
        notifyStateChange(task, eventAction, recipientEmail);
    }
}