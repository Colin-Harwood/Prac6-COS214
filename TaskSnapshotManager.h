#ifndef TASKSNAPSHOTMANAGER_H
#define TASKSNAPSHOTMANAGER_H

#include <stack>
#include "Task.h"

using namespace std;

class TaskSnapshotManager{
    private:
    stack<Task*> history;

    public:
    TaskSnapshotManager();
    ~TaskSnapshotManager();
    void addSnapshot(Task* task); //add to the history stack
    Task* getSnapshot(); //returns a task and pops from the history stack
};

#endif // TASKSNAPSHOTMANAGER_H