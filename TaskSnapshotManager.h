/**
 * @file TaskSnapshotManager.h
 * @brief Manages task history snapshots for undo and rollback.
 */

#ifndef TASKSNAPSHOTMANAGER_H
#define TASKSNAPSHOTMANAGER_H

#include <stack>
#include "Task.h"

using namespace std;

/**
 * @brief Stores task snapshots and restores earlier task states.
 */
class TaskSnapshotManager {
    private:
    stack<Task*> history;

    public:
    /// @brief Creates an empty snapshot history.
    TaskSnapshotManager();

    /// @brief Deletes all snapshots in the history.
    ~TaskSnapshotManager();

    /// @brief Adds a clone of a task to the history.
    void addSnapshot(Task* task);

    /// @brief Saves a task snapshot; equivalent to addSnapshot().
    void save(Task* task);

    /**
     * @brief Restores a task from the latest snapshot.
     * @return True if a snapshot was restored, otherwise false.
     */
    bool restoreLatest(Task* t);

    /**
     * @brief Removes and returns the latest snapshot, or nullptr if history is empty.
     * The caller owns the returned snapshot.
     */
    Task* getSnapshot();
};

#endif // TASKSNAPSHOTMANAGER_H
