#ifndef CONCRETEHANDLERC_H
#define CONCRETEHANDLERC_H
#include <string>
#include "Handler.h"

/**
 * @class ConcreteHandlerA
 * @brief Concrete Handler: handles tasks whose code matches its own
 */
class ConcreteHandlerC :public Handler{
private:
    std::string code;
public:
    explicit ConcreteHandlerC(std::string code);
    bool canHandle(Task* task) override;
    void process(Task* task) override;
};
#endif