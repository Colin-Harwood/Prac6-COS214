#ifndef COMMAND_H
#define COMMAND_H

#include <string>

// Forward declarations to avoid circular dependencies
class Task;
class LogQueue;
class TaskSnapshotManager;

class Command {
public:
    /**
     * @brief Constructs a new Command base object.
     *
     * @param q Pointer to the LogQueue that stores executed command history.
     * @param s Pointer to the TaskSnapshotManager that manages rollback snapshots.
     */
    Command(LogQueue* q, TaskSnapshotManager* s);
    virtual ~Command() = default;

    virtual void execute(Task* t) = 0;
    virtual void undo(Task* t) = 0;
    virtual Command* clone() const = 0; // Prototype pattern

    /**
     * @brief Describes the command action and associated task.
     *
     * @return std::string Formatted string representing the action and task name (e.g., "ACTION: TaskName").
     */
    std::string describe() const;

protected:
    /**
     * @brief Records the command execution and logs a frozen clone to the queue.
     *
     * Sets the current task name and action on this command, then pushes an
     * independent clone of this command to the log queue.
     *
     * @param t Pointer to the task being recorded.
     * @param act The description of the action being logged.
     */
    void record(Task* t, const std::string& act);
    // Non-owning pointers
    LogQueue* logQueue;
    TaskSnapshotManager* snapshotManager;

    std::string taskName;
    std::string action;
};

#endif
