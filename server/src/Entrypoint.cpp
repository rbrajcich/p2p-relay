#include <csignal>

#include "Logger.h"
#include "SessionTable.h"

bool running = true;

// Handle signal that process should terminate
void handleSIGTERM(int signal) {
    running = false;
}

int entry() {

    Logger &logger = Logger::getGlobalLogger();

    // Register SIGTERM handler
    signal(SIGTERM, handleSIGTERM);

    logger.info("*********************************************");
    logger.info("**        Starting P2P Relay Server        **");
    logger.info("*********************************************");

    SessionTable *sessionTable = new SessionTable();

    while (running) {
        // Do nothing
    }

    delete sessionTable;
    return 0;
}
