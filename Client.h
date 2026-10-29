// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

class TividyFacade;

/**
 * Client = the TUI. It only ever talks to the TividyFacade.
 */
class Client {
private:
    TividyFacade* facade;   // not owned

public:
    explicit Client(TividyFacade* facade);
    // Reads menu choices from in until the user quits (streams can be swapped for tests)
    void run(std::istream& in = std::cin, std::ostream& out = std::cout);
};

#endif
