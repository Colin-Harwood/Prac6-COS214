#include "LogQueue.h"
#include "Command.h"


void LogQueue::push(Command* c) {
    q.push(c);
}


void LogQueue::flushTo(std::ostream& out) {
    while (!q.empty()) {
        out << q.front()->describe() << "\n";
        delete q.front(); // Queue owns the command, so it deletes it
        q.pop();
    }
}


bool LogQueue::empty() const {
    return q.empty();
}

/*
 * NEEDED BY THE taskSnapshotManager
 * w// Takes a clone of the task (Prototype B)
   void save(Task* t);

   // Restores fields from the latest snapshot. Returns true if successful.
   bool restoreLatest(Task* t);
*/
