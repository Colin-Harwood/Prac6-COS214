#ifndef TARGET_H
#define TARGET_H

#include <string>

class Task;

class Target
{
protected:
    std::string recipientEmail;

public:
    /**
     * @brief Constructs a Target interface with an optional default recipient email.
     * @param defaultRecipient Initial destination email address.
     */
    explicit Target(const std::string &defaultRecipient = "");

    /**
     * @brief Virtual destructor for polymorphic cleanup.
     */
    virtual ~Target();

    /**
     * @brief Updates the recipient email address captured from the GUI input field.
     * @param email Destination email address string.
     */
    void setRecipientEmail(const std::string &email);

    /**
     * @brief Retrieves the currently stored recipient email address.
     * @return The configured recipient email string.
     */
    std::string getRecipientEmail() const;

    /**
     * @brief Sends an email notification when an individual task finishes executing.
     * @param task Pointer to the completed Task.
     * @param recipientEmail Destination email address.
     */
    virtual void notifyTaskComplete(Task *task, std::string recipientEmail) = 0;
    
    /**
     * @brief Sends an email notification containing the full audit log when a project closes.
     * @param project Pointer to the root project Task.
     * @param auditLog Compiled workflow execution log from LogQueue.
     * @param recipientEmail Destination email address.
     */
    virtual void notifyProjectComplete(Task *project, std::string auditLog, std::string recipientEmail) = 0;

    /**
     * @brief Sends an alert email when a task undergoes a critical state transition.
     * @param task Pointer to the affected Task.
     * @param eventAction Description of the state change.
     * @param recipientEmail Destination email address.
     */
    virtual void notifyStateChange(Task *task, const std::string &eventAction, const std::string &recipientEmail) = 0;
    
    /**
     * @brief Convenience overload that notifies task completion using the stored GUI recipientEmail.
     * @param task Pointer to the completed Task.
     */
    void notifyTaskComplete(Task *task);

    /**
     * @brief Convenience overload that notifies project completion using the stored GUI recipientEmail.
     * @param project Pointer to the root project Task.
     * @param auditLog Compiled workflow execution log.
     */
    void notifyProjectComplete(Task *project, const std::string &auditLog);
    
    /**
     * @brief Convenience overload that notifies a state change using the stored GUI recipientEmail.
     * @param task Pointer to the affected Task.
     * @param eventAction Description of the state transition.
     */
    void notifyStateChange(Task *task, const std::string &eventAction);
};

#endif