#include "LogCommand.h"
#include "Task.h"
#include "LogQueue.h"

LogCommand::LogCommand(LogQueue* l, TaskSnapshotManager* s, const std::string& fname)
    : Command(l, s), filename(fname) {
    // Open file in append mode
    sink = std::unique_ptr<std::ofstream>(new std::ofstream(fname.c_str(), std::ios::app));
}


void LogCommand::execute(Task* t) {
    record(t, "WORKFLOW CLOSED");
    flushTo();
}


void LogCommand::undo(Task* t) {
    // A flush to a file is irreversible
    (void)t; // Suppress unused parameter warning
}


Command* LogCommand::clone() const {
    // Clone creates a new LogCommand pointing to the same file
    LogCommand* copy = new LogCommand(logQueue, snapshotManager, filename);
    copy->taskName = this->taskName;
    copy->action = this->action;
    return copy;
}


void LogCommand::flushTo() {
    if (sink && sink->is_open()) {
        logQueue->flushTo(*sink);
        sink->flush();
    }
}
