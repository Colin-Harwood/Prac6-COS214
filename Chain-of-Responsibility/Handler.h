#ifndef HANDLER_H
#define HANDLER_H

#include "Task.h"

class Command;

/**
 * @file Handler.h
 * @brief Handler of the Chain of Responsibility pattern.
 * Handels the tasks formed by the compsoite
 */

class Handler{
protected:
    Handler* next;
    Command* command;
public:
    Handler();
    virtual ~Handler();
    virtual bool canHandle(Task* task) = 0;
    virtual void process(Task* t) = 0;
    void setNext(Handler* h);
    void handle(Task* t);
};

#endif