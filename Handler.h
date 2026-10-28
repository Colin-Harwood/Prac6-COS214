#ifndef HANDLER_H
#define HANDLER_H

class Task;
class Command;

/**
 * Handler (Chain of Responsibility)
 * Handles a task if its category is inside [minLeaf, maxLeaf]
 * otherwise passes it to the next handler
 */
class Handler {
private:
    Handler* next;
    Command* command;//command
protected:
    int minLeaf;
    int maxLeaf;
    virtual void process(Task* t, int number) = 0;
public:
    explicit Handler(Command* c = nullptr);
    virtual ~Handler();
    bool canHandle(int number) const;
    void setNext(Handler* h);
    void handle(Task* t, int number);
};
#endif