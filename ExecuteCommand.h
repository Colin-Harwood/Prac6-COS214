#ifndef EXECUTE_COMMAND_H
#define EXECUTE_COMMAND_H

#include "Command.h"

class ExecuteCommand : public Command {
public:
    /**
     * @brief Constructs a new ExecuteCommand object.
     *
     * @param q Pointer to the LogQueue used for logging executed commands.
     * @param s Pointer to the TaskSnapshotManager used for task state snapshots.
     */
    ExecuteCommand(LogQueue* q, TaskSnapshotManager* s);

    /**
     * @brief Saves a task snapshot, starts and completes the task, and records the result.
     *
     * Captures a snapshot of the task prior to execution. Transitions the task through
     * start and complete states. If an exception occurs, restores the task from the latest
     * snapshot, records a "FAILED" entry, and re-throws the exception. Otherwise records
     * an "EXECUTED" entry upon success.
     *
     * @param t Pointer to the Task object to be executed.
     */
    void execute(Task* t) override;

    /**
     * @brief Reverts the task to its previous state before execution.
     *
     * Restores the most recent snapshot of the given task from the snapshot manager
     * and records an "UNDONE" entry to the log queue.
     *
     * @param t Pointer to the Task object to be reverted.
     */
    void undo(Task* t) override;

    /**
     * @brief Creates a cloned instance of this ExecuteCommand.
     *
     * @return Command* A pointer to the newly created ExecuteCommand copy.
     */
    Command* clone() const override;
};

#endif
