#include "TaskSnapshotManager.h"

TaskSnapshotManager::~TaskSnapshotManager() {
    while (!history.empty()) {
        history.pop();
    }
}

void TaskSnapshotManager::addSnapshot(Task* task) {
    history.push(task);
}

Task* TaskSnapshotManager::getSnapshot(){
    if (history.empty()) {
        return nullptr;
    }
    Task* task = history.top();
    history.pop();
    return task;
}

