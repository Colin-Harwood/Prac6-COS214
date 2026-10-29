#include "TaskSnapshotManager.h"

TaskSnapshotManager::TaskSnapshotManager() {}

TaskSnapshotManager::~TaskSnapshotManager() {
    while (!history.empty()) {
        delete history.top();
        history.pop();
    }
}

///quick fix: maybe check -KAI
void TaskSnapshotManager::save(Task* task) {
    addSnapshot(task);
}

void TaskSnapshotManager::addSnapshot(Task* task) {
    history.push(task->clone());
}

Task* TaskSnapshotManager::getSnapshot(){
    if (history.empty()) {
        return nullptr;
    }
    Task* task = history.top();
    history.pop();
    return task;
}

////ADDED BY KAI
bool TaskSnapshotManager::restoreLatest(Task* t) {
    if (!t || history.empty()) {
        return false;
    }

    Task* snapshot = history.top();
    history.pop();

    // Context Rule: "Restoring should copy fields back rather than swap pointers."
    t->restoreFrom(snapshot);

    // Clean up the snapshot after its fields have been safely copied back
    delete snapshot;

    return true;
}
