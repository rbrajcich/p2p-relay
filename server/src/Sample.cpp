#include <iostream>
#include <csignal>
#include <unistd.h>

#include "Sample.h"

bool running = true;

// Handle signal that process should terminate
void handleSIGTERM(int signal) {
    running = false;
}

int entry() {

    // Register SIGTERM handler
    signal(SIGTERM, handleSIGTERM);

    int i = 0;

    while (running) {
        std::cout << "Running " << ++i << " seconds" << std::endl;
        usleep(1000000);
    }

    return 0;
}

int addIntegers(int x, int y) {
    return x + y;
}
