#include "Util.h"

#include <cstdlib>

#include "Logger.h"

bool Util::getUint32FromEnv(const char *envVarName, uint32_t *val) {
    
    Logger &logger = Logger::getGlobalLogger();
    char *rawStr, *endptr;

    rawStr = std::getenv(envVarName);
    if (rawStr == nullptr) {
        logger.info("Environment variable " + std::string(envVarName) +
                    " not found.");
        return false;
    }

    *val = (uint32_t) strtoul(rawStr, &endptr, 10);
    if (rawStr == endptr) {
        logger.info("Unable to parse " + std::string(envVarName) +
                    "'s value: " + rawStr);
        return false;
    }

    return true;
}