/**
 * @file TividyFacade.h
 * @brief Single entry point to the TIVIDY workflow engine.
 *
 * Role in the Facade pattern: Facade. The GUI uses this class instead of creating
 * and connecting the Builder, Composite, Chain of Responsibility, Command and
 * Adapter classes itself.
 */
#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H

#include <string>
#include <vector>
#include "TaskInput.h"

class Director;
class Task;
class Handler;
class Command;
class LogCommand;
class LogQueue;
class TaskSnapshotManager;
class Target;

/**
 * @brief Creates and connects the workflow engine, and gives the client simple calls to use it.
 */
class TividyFacade {
private:
    /**
     * @brief Directs building. Only set during finishWorkflow().
     */
    Director* director;
    /**
     * @brief Top of the current workflow, owned by the facade.
     */
    Task* root;
    /**
     * @brief First handler in the chain.
     */
    Handler* chain;
    /**
     * @brief Log entries waiting to be saved, shared by every command.
     */
    LogQueue* logQueue;
    /**
     * @brief Task snapshots used for undo, shared by every command.
     */
    TaskSnapshotManager* snapshotManager;
    /**
     * @brief Email service, owned by the facade. Null until setNotifier() is called.
     */
    Target* notifier;

    /**
     * @brief Every handler, kept so the facade can delete them.
     */
    std::vector<Handler*> handlers;
    /**
     * @brief ExecuteCommand shared by the handlers and completeTask().
     */
    Command* command;
    /**
     * @brief Closes and saves the log. Used by the handlers, owned by the facade.
     */
    LogCommand* logCommand;
    /**
     * @brief Stack of open groups while a workflow is being created.
     */
    std::vector<TaskInput> pending;
    /**
     * @brief Tasks that changed, in order, so undoLast() knows which to restore.
     */
    std::vector<Task*> undoTargets;
    /**
     * @brief Whether the start email has been handled for this workflow.
     */
    bool workflowStarted;

    /**
     * @brief Creates the commands and handlers, and links the chain.
     */
    void buildChain();
    /**
     * @brief Finds a task in the current workflow.
     *
     * @param taskName Name of the task or group.
     * @return The task, or nullptr if it is not found.
     */
    Task* findTask(const std::string& taskName);

    /**
     * @brief Searches a subtree for a task.
     *
     * @param node Top of the subtree.
     * @param taskName Name of the task or group.
     * @return The task, or nullptr if it is not found.
     */
    Task* findIn(Task* node, const std::string& taskName);
    /**
     * @brief Gathers the department level of every leaf.
     *
     * @param node Top of the subtree.
     * @param levels Receives the levels, which may repeat.
     */
    void collectLevels(Task* node, std::vector<int>& levels);
    /**
     * @brief Gathers every leaf task.
     *
     * @param node Top of the subtree.
     * @param leaves Receives the leaf tasks.
     */
    void collectLeaves(Task* node, std::vector<Task*>& leaves);
    /**
     * @brief Prints a task and its children.
     *
     * @param node Task to print.
     * @param depth Indent level.
     */
    void printNode(Task* node, int depth);
    /**
     * @brief Sends one department level through the chain and records changed tasks.
     *
     * @param level Department level to run.
     * @return The number of tasks that were eligible to run.
     */
    int dispatchLevel(int level);
    /**
     * @brief Sends the start email, once per workflow.
     */
    void sendStartEmail();
    /**
     * @brief Sends the finish email, including the contents of workflow.log.
     */
    void sendCompletionEmail();
    /**
     * @brief Moves one task to a new state, with a snapshot, undo record and log entry.
     *
     * @param taskName Name of the task.
     * @param move Task action to call.
     * @param action Text for the log entry.
     * @return True if the task moved. False if it is missing, is a group, or the move is refused.
     */
    bool transition(const std::string& taskName, void (Task::*move)(), const std::string& action);
    /**
     * @brief Closes the log and sends the finish email if every task is finished.
     */
    void closeLogIfFinished();

public:
    /**
     * @brief Creates the log queue and snapshot manager, and builds the chain.
     */
    TividyFacade();
    /**
     * @brief Deletes everything the facade owns, including the notifier and workflow.
     */
    ~TividyFacade();

    /**
     * @brief Placeholder: loading from a file is not supported yet.
     *
     * @param file Path that was requested.
     * @return Always false.
     */
    bool loadWorkflow(const std::string& file);
    /**
     * @brief Runs every department level in order.
     *
     * Sends the start email first, and the finish email if this run finishes the workflow.
     */
    void runWorkflow();
    /**
     * @brief Prints the workflow tree with each task's state.
     */
    void printWorkflow();
    /**
     * @brief Gets the state of a task or group.
     *
     * @param taskName Name of the task or group.
     * @return The state name, or "Task not found".
     */
    std::string getTaskStatus(const std::string& taskName);
    /**
     * @brief Moves a task to InProgress.
     *
     * @param taskName Name of the task.
     * @return True if it moved. False if it is missing, is a group, or the move is refused.
     */
    bool startTask(const std::string& taskName);
    /**
     * @brief Completes a task.
     *
     * A Pending or TimedOut task runs through the command, as it would in the chain.
     *
     * @param taskName Name of the task.
     * @return True if it completed. False if it is missing, is a group, or the move is refused.
     */
    bool completeTask(const std::string& taskName);
    /**
     * @brief Moves a task to Rejected.
     *
     * @param taskName Name of the task.
     * @return True if it moved. False if it is missing, is a group, or the move is refused.
     */
    bool rejectTask(const std::string& taskName);
    /**
     * @brief Moves a task to Cancelled.
     *
     * @param taskName Name of the task.
     * @return True if it moved. False if it is missing, is a group, or the move is refused.
     */
    bool cancelTask(const std::string& taskName);
    /**
     * @brief Moves a task back to Pending.
     *
     * @param taskName Name of the task.
     * @return True if it moved. False if it is missing, is a group, or the move is refused.
     */
    bool reopenTask(const std::string& taskName);
    /**
     * @brief Restores the most recently changed task.
     *
     * @return True if a task was restored.
     */
    bool undoLast();
    /**
     * @brief Saves the log queue to workflow.log, then prints the file.
     */
    void showLog();
    /**
     * @brief Reruns every level that has a timed-out task.
     *
     * Pending tasks at those levels run as well.
     */
    void checkTimeouts();

    /**
     * @brief Replaces the current workflow and resets undo and email tracking.
     *
     * @param t New workflow, which the facade takes ownership of.
     */
    void setRoot(Task* t);

    /**
     * @brief Starts creating a workflow, discarding any unfinished one.
     *
     * @param name Workflow name.
     * @param deadline Workflow deadline.
     * @return Always true.
     */
    bool beginWorkflow(const std::string& name, const std::string& deadline);
    /**
     * @brief Opens a nested group.
     *
     * @param name Group name.
     * @param deadline Group deadline.
     * @return False if no workflow has begun.
     */
    bool beginGroup(const std::string& name, const std::string& deadline);
    /**
     * @brief Adds a task to the open group.
     *
     * @param name Task name.
     * @param deadline Task deadline.
     * @param deptLevel Department level, from 1 to 9.
     * @param duration Task duration.
     * @return False if no workflow has begun or the level is out of range.
     */
    bool addTask(const std::string& name, const std::string& deadline, int deptLevel, int duration);
    /**
     * @brief Closes the open group and adds it to its parent.
     *
     * @return False if no group is open.
     */
    bool endGroup();
    /**
     * @brief Builds the workflow through the Director and makes it current.
     *
     * @return False if no workflow has begun or a group is still open.
     */
    bool finishWorkflow();

    /**
     * @brief Connects the email service.
     *
     * @param t Notifier, which the facade takes ownership of.
     */
    void setNotifier(Target* t);
    /**
     * @brief Sets who receives the emails.
     *
     * @param email Recipient address.
     * @return False if the address is empty or no notifier is connected.
     */
    bool setNotificationEmail(const std::string& email);
};

#endif
