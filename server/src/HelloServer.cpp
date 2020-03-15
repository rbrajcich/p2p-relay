#include <iostream>
#include <csignal>

bool running = true;

// Handle signal that process should terminate
void handleSIGTERM(int signal) {
    running = false;
}

int entry() {

    // Register SIGTERM handler
    signal(SIGTERM, handleSIGTERM);

#ifndef P2P_RELAY_UNIT_TEST
    std::cout << "Hello From Server!" << std::endl;
#endif

    return 0;
}
