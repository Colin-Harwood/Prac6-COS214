// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#include "Client.h"
#include "TividyFacade.h"
#include <string>

Client::Client(TividyFacade* facade) : facade(facade) {}

void Client::run(std::istream& in, std::ostream& out) {
    std::string choice;
    while (true) {
        out << "\n=== TIVIDY ===\n"
            << " 1. Load workflow     2. Run workflow     3. Show workflow\n"
            << " 4. Task status       5. Start task       6. Complete task\n"
            << " 7. Reject task       8. Cancel task      9. Reopen task\n"
            << "10. Undo last        11. Show log        12. Check timeouts\n"
            << " 0. Quit\n> ";
        if (!std::getline(in, choice) || choice == "0") {
            return;
        }
        if (choice == "2")  { facade->runWorkflow(); continue; }
        if (choice == "3")  { facade->printWorkflow(); continue; }
        if (choice == "10") { facade->undoLast(); continue; }
        if (choice == "11") { facade->showLog(); continue; }
        if (choice == "12") { facade->checkTimeouts(); continue; }

        std::string arg;
        out << (choice == "1" ? "File: " : "Task name: ");
        if (!std::getline(in, arg)) {
            return;
        }
        if      (choice == "1") facade->loadWorkflow(arg);
        else if (choice == "4") out << facade->getTaskStatus(arg) << "\n";
        else if (choice == "5") facade->startTask(arg);
        else if (choice == "6") facade->completeTask(arg);
        else if (choice == "7") facade->rejectTask(arg);
        else if (choice == "8") facade->cancelTask(arg);
        else if (choice == "9") facade->reopenTask(arg);
        else out << "Unknown option.\n";
    }
}
