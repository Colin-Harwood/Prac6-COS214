#include "Command.h"
#include "Snapshot.h"

Command::~Command() {
	// TODO - delete any snapshots still on the stack (destructors must not throw)
}

std::string Command::getState() {
	// TODO - implement Command::getState
	throw "Not yet implemented";
}

void Command::writeToLogFile() {
	// TODO - implement Command::writeToLogFile
	throw "Not yet implemented";
}

void Command::storeSnapshot(Snapshot* s) {
	// TODO - implement Command::storeSnapshot
	throw "Not yet implemented";
}

Snapshot* Command::retrieveSnapshot() {
	// TODO - implement Command::retrieveSnapshot
	throw "Not yet implemented";
}
