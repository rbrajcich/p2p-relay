#include "Util.h"

#include "Logger.h"

static Logger &logger = Logger::getGlobalLogger();

bool Util::getUint32FromEnv(const char *envVarName, uint32_t *val) {
    
    char *rawStr, *endptr;

    rawStr = std::getenv(envVarName);
    if (rawStr == nullptr) {
        logger.debug("Environment variable " + std::string(envVarName) +
                     " not found.");
        return false;
    }

    *val = (uint32_t) strtoul(rawStr, &endptr, 10);
    if (rawStr == endptr) {
        logger.error("Unable to parse " + std::string(envVarName) +
                     "'s value: " + rawStr);
        return false;
    }

    return true;
}

bool Util::getLowercaseStrFromEnv(const char *envVarName, std::string *val) {
    
    char *rawStr;
    int i;

    rawStr = std::getenv(envVarName);
    if (rawStr == nullptr) {
        return false;
    }

    i = 0;
    while (rawStr[i] != '\0') {
        if (rawStr[i] >= 'A' && rawStr[i] <= 'Z') {
            rawStr[i] += ('a' - 'A');
        }
        i++;
    }

    val->assign(rawStr);
    return true;
}