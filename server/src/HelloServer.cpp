#include <iostream>

int entry() {

#ifndef P2P_RELAY_UNIT_TEST
    std::cout << "Hello From Server!" << std::endl;
#endif
    return 0;
}
