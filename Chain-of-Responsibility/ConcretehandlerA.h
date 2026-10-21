#ifndef CONCRETEHANDLERA_H
#define CONCRETEHANDLERA_H
#include <string>
#include "Handler.h"

/**
 * @class ConcreteHandlerA
 * @brief Concrete Handler: handles tasks whose code matches its own
 */
class ConcreteHandlerA :public Handler{
private:
    std::string code;
public:
    explicit ConcreteHandlerA(std::string code);
    bool canHandle(Task* task) override;
    void process(Task* task) override;
};
#endif