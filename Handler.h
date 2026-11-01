/**
 * @file Handler.h
 * @brief Base Handler for the Chain of Responsibility pattern.
 */

#ifndef HANDLER_H
#define HANDLER_H

class Task;
class Command;
class LogCommand;

/**
 * @brief Handler in the Chain of Responsibility pattern.
 * Handles a task if its category is inside [minLeaf, maxLeaf], otherwise passes it to the next handler.
 */
class Handler {
private:
    Handler* next;

protected:
    int minLeaf;
    int maxLeaf;
    Command* command;
    LogCommand* logcommand;

    virtual void process(Task* t, int number) = 0;
    /**
     * @brief Runs the task; returns true if the task ended Completed.
     */
    bool runTask(Task* task, const char* label);
public:
    explicit Handler(Command* c = nullptr);
    virtual ~Handler();
    bool canHandle(int number) const;
    void setNext(Handler* h);
    void setLogCommand(LogCommand* l);
    void handle(Task* t, int number);
};
#endif
