/**
 * @file Target.h
 * @brief Abstract Target interface for notification adapters.
 */

#ifndef TARGET_H
#define TARGET_H

#include <string>

class Task;

/**
 * @class Target
 * @brief Abstract Target interface used by Command objects to trigger workflow email notifications.
 *
 * Stores the user's destination email address captured from the GUI and declares
 * domain-specific notification hooks for task completion, project closure, and
 * critical state transitions.
 */
class Target
{
protected:
    std::string recipientEmail; /**< Destination email address entered by the user in the GUI */

public:
    /**
     * @brief Constructs a Target with an optional default recipient email address.
     * @param defaultRecipient Initial recipient email address string.
     */
    explicit Target(const std::string &defaultRecipient = "");

    /**
     * @brief Virtual destructor for polymorphic cleanup.
     */
    virtual ~Target();

    /**
     * @brief Dynamically updates the recipient email address from the GUI input field.
     * @param email The user's email address string.
     */
    void setRecipientEmail(const std::string &email);

    /**
     * @brief Retrieves the currently configured recipient email address.
     * @return Stored recipient email string.
     */
    std::string getRecipientEmail() const;

    /**
     * @brief Sends an email notification when a workflow task completes.
     * @param task Pointer to the completed Task.
     * @param recipientEmail Destination email address.
     */
    virtual void notifyTaskComplete(Task *task, std::string recipientEmail) = 0;

    /**
     * @brief Sends an email notification with the full workflow audit log when a project completes.
     * @param project Pointer to the root project Task.
     * @param auditLog Compiled execution history flushed from LogQueue.
     * @param recipientEmail Destination email address.
     */
    virtual void notifyProjectComplete(Task *project, std::string auditLog, std::string recipientEmail) = 0;

    /**
     * @brief Sends an alert email when a task undergoes an important state transition.
     * @param task Pointer to the Task whose state changed.
     * @param eventAction Description of the state change event (e.g., "TIMED OUT", "FAILED", "UNDONE").
     * @param recipientEmail Destination email address.
     */
    virtual void notifyStateChange(Task *task, const std::string &eventAction, const std::string &recipientEmail) = 0;
    
    /**
     * @brief Convenience overload that notifies task completion using the stored recipientEmail.
     * @param task Pointer to the completed Task.
     */
    void notifyTaskComplete(Task *task);

    /**
     * @brief Convenience overload that notifies project completion using the stored recipientEmail.
     * @param project Pointer to the root project Task.
     * @param auditLog Compiled execution history string.
     */
    void notifyProjectComplete(Task *project, const std::string &auditLog);

    /**
     * @brief Convenience overload that notifies a task state change using the stored recipientEmail.
     * @param task Pointer to the affected Task.
     * @param eventAction Description of the state transition.
     */
    void notifyStateChange(Task *task, const std::string &eventAction);
};

#endif
