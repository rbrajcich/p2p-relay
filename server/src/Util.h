#ifndef P2P_RELAY__UTIL_H
#define P2P_RELAY__UTIL_H

#include <cstdint>

class Util {
 public:

    /*
     * This function attempts to extract a uint32 value from
     * the environment the program is executing in. If successful,
     * the value extracted is placed in *val and true is returned.
     * If unsuccessful, returns false.
     */
    static bool getUint32FromEnv(const char *envVarName, uint32_t *val);

 private:

    /*
     * Private constructor as Util should not ever be instantiated
     */
    Util();
};

#endif
