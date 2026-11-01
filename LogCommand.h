#ifndef LOG_COMMAND_H
#define LOG_COMMAND_H

#include "Command.h"
#include <string>
#include <memory>
#include <fstream>

class LogCommand : public Command {
public:
    /**
     * @brief Constructs a new LogCommand object and opens the log file.
     *
     * @param l Pointer to the LogQueue containing logged commands.
     * @param s Pointer to the TaskSnapshotManager.
     * @param fname File path of the log output file to open in append mode.
     */
    LogCommand(LogQueue* l, TaskSnapshotManager* s, const std::string& fname);
    ~LogCommand() override = default;

    /**
     * @brief Records workflow completion and flushes the queue to the log file.
     *
     * Marks the workflow as closed by recording "WORKFLOW CLOSED" and writes all
     * pending logged commands out to the target file.
     *
     * @param t Pointer to the final task being finalized.
     */
    void execute(Task* t) override;

    /**
     * @brief No-op implementation for workflow undo.
     *
     * Flushes to a persistent file cannot be undone; this method safely ignores the operation.
     *
     * @param t Pointer to the Task object (unused).
     */
    void undo(Task* t) override;

    /**
     * @brief Creates a cloned instance of this LogCommand.
     *
     * @return Command* A pointer to the newly allocated LogCommand copy.
     */
    Command* clone() const override;

    /**
     * @brief Writes the contents of the LogQueue out to the file sink.
     *
     * Verifies that the file stream is valid and open, then flushes all queued
     * log entries into the stream.
     */
    void flushTo();

private:
    // Owns the file stream
    std::unique_ptr<std::ofstream> sink;
    std::string filename;
};

#endif
