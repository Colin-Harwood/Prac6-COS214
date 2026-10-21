#ifndef CONCRETEHANDLERB_H
#define CONCRETEHANDLERB_H
#include <string>
#include "Handler.h"

/**
 * @class ConcreteHandlerB
 * @brief Concrete Handler: handles tasks whose code matches its own
 */
class ConcreteHandlerB :public Handler{
private:
    std::string code;
public:
    explicit ConcreteHandlerB(std::string code);
    bool canHandle(Task* task) override;
    void process(Task* task) override;
};
#endif