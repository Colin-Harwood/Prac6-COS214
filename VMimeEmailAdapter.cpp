#include "VMimeEmailAdapter.h"
#include "VMimeMailer.h"
#include "Task.h"
#include <sstream>

VMimeEmailAdapter::VMimeEmailAdapter(VMimeMailer *mailer,
                                     std::string sender,
                                     const std::string &defaultRecipient)
    : Target(defaultRecipient),
      adaptee(mailer),
      senderAddress(sender)
{
}

VMimeEmailAdapter::~VMimeEmailAdapter()
{
}

std::string VMimeEmailAdapter::buildTaskSubject(Task *task)
{
    if (task == nullptr)
    {
        return "[TIVIDY] Task Notification";
    }
    std::ostringstream oss;
    oss << "[TIVIDY] Task '" << task->getName()
        << "' (Dept " << task->getDepartmentLevel()
        << ") - State: " << task->getStateName();
    return oss.str();
}

std::string VMimeEmailAdapter::buildTaskBody(Task *task)
{
    if (task == nullptr)
    {
        return "No task details available.";
    }
    std::ostringstream oss;
    oss << "Hello,\n\n"
        << "Here is an update regarding your workflow task in TIVIDY:\n"
        << "  - Task Name        : " << task->getName() << "\n"
        << "  - Department Level : " << task->getDepartmentLevel() << "\n"
        << "  - Deadline         : " << task->getDeadline() << "\n"
        << "  - Current State    : " << task->getStateName() << "\n"
        << "  - Finished Status  : " << (task->isFinished() ? "Yes" : "No") << "\n\n"
        << "Regards,\nTIVIDY Workflow Engine";
    return oss.str();
}

void VMimeEmailAdapter::notifyTaskComplete(Task *task, std::string recipientEmail)
{
    if (adaptee == nullptr || task == nullptr)
    {
        return;
    }

    std::string targetEmail = recipientEmail.empty() ? this->recipientEmail : recipientEmail;
    if (targetEmail.empty())
    {
        return;
    }

    if (adaptee->connectSMTP(adaptee->getSmtpHost(), adaptee->getSmtpPort()))
    {
        std::string subject = buildTaskSubject(task);
        std::string body = buildTaskBody(task);
        adaptee->sendMimeMessage(senderAddress, targetEmail, subject, body);
    }
}

void VMimeEmailAdapter::notifyProjectComplete(Task *project, std::string auditLog, std::string recipientEmail)
{
    if (adaptee == nullptr)
    {
        return;
    }

    std::string targetEmail = recipientEmail.empty() ? this->recipientEmail : recipientEmail;
    if (targetEmail.empty())
    {
        return;
    }

    std::string projectName = (project != nullptr) ? project->getName() : "TIVIDY Project";
    std::string subject = "[TIVIDY] Project Completed: " + projectName;

    std::ostringstream bodyStream;
    bodyStream << "Hello,\n\n"
               << "The workflow project '" << projectName << "' has been declared complete.\n";
    if (project != nullptr)
    {
        bodyStream << "Final Project State: " << project->getStateName() << "\n"
                   << "Deadline: " << project->getDeadline() << "\n";
    }
    bodyStream << "\n--- WORKFLOW AUDIT LOG ---\n"
               << (auditLog.empty() ? "(No audit entries recorded)" : auditLog) << "\n"
               << "--------------------------\n\n"
               << "Regards,\nTIVIDY Workflow Engine";

    if (adaptee->connectSMTP(adaptee->getSmtpHost(), adaptee->getSmtpPort()))
    {
        adaptee->sendMimeMessage(senderAddress, targetEmail, subject, bodyStream.str());
    }
}

void VMimeEmailAdapter::notifyStateChange(Task *task, const std::string &eventAction, const std::string &recipientEmail)
{
    if (adaptee == nullptr || task == nullptr)
    {
        return;
    }

    std::string targetEmail = recipientEmail.empty() ? this->recipientEmail : recipientEmail;
    if (targetEmail.empty())
    {
        return;
    }

    if (adaptee->connectSMTP(adaptee->getSmtpHost(), adaptee->getSmtpPort()))
    {
        std::string subject = "[TIVIDY] Alert (" + eventAction + "): " + buildTaskSubject(task);
        std::string body = "Event Triggered: " + eventAction + "\n\n" + buildTaskBody(task);
        adaptee->sendMimeMessage(senderAddress, targetEmail, subject, body);
    }
}