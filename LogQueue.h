/**
 * @file LogQueue.h
 * @brief Queue for storing and flushing executed commands.
 */

#ifndef LOG_QUEUE_H
#define LOG_QUEUE_H

#include <queue>
#include <iostream>
#include <string>

class Command; // Forward declaration

/**
 * @brief Queue for managing executed command history.
 */
class LogQueue {
public:

    LogQueue() = default;
    ~LogQueue();

    /**
     * @brief Pushes a command to the queue.
     *
     * The queue takes ownership of the command pointer and will free it during flush.
     *
     * @param c Pointer to the Command to be enqueued.
     */
    void push(Command* c);

    /**
     * @brief Flushes and deletes all commands from the queue in FIFO order.
     *
     * Writes the string representation of each command to the given output stream
     * and deletes each command object.
     *
     * @param out The target output stream (e.g. std::cout or std::ofstream).
     */
    void flushTo(std::ostream& out);

    /**
     * @brief Checks whether the log queue is currently empty.
     *
     * @return true If there are no commands in the queue.
     * @return false If the queue contains one or more commands.
     */
    bool empty() const;

private:
    std::queue<Command*> q;
};

#endif
