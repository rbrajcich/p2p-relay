#include "Logger.h"

#include <iostream>

#include "Util.h"

/* 
 * The environment variable that can be set to override the
 * default log level "Info". debug | info | error | none
 */
#define LOGGER_LOG_LEVEL_ENV_VAR "P2P_SERVER_LOG_LEVEL"

Logger *Logger::loggerInst = nullptr;

#if defined(P2P_RELAY_UNIT_TEST) || defined(P2P_RELAY_SYS_TEST)
std::ofstream Logger::logOutputFile;
#endif

Logger::Logger() {

    bool foundInEnv = false;
    std::string envVal;

    #if defined(P2P_RELAY_UNIT_TEST)
    if (!logOutputFile.is_open()) {
        logOutputFile.open(UNIT_TEST_LOG_OUTPUT_FILE);
    }
    logs = "";
    #elif defined(P2P_RELAY_SYS_TEST)
    if (!logOutputFile.is_open()) {
        logOutputFile.open(SYS_TEST_LOG_OUTPUT_FILE);
    }
    #endif

    foundInEnv = Util::getLowercaseStrFromEnv(LOGGER_LOG_LEVEL_ENV_VAR,
                                              &envVal);

    /* LCOV_EXCL_START */
    currentLogLevel = LogLevel::Debug;
    if (foundInEnv) {
        if (envVal == "debug") {
            log(LogLevel::Debug,
                "\nStarting logger at 'Debug' level from env variable\n");
            currentLogLevel = LogLevel::Debug;
        } else if (envVal == "info") {
            log(LogLevel::Debug,
                "\nStarting logger at 'Info' level from env variable\n");
            currentLogLevel = LogLevel::Info;
        } else if (envVal == "error") {
            log(LogLevel::None,
                "\nStarting logger at 'Error' level from env variable\n");
            currentLogLevel = LogLevel::Error;
        } else if (envVal == "none") {
            log(LogLevel::Debug,
                "\nLogger configured to 'None' by env variable\n");
            currentLogLevel = LogLevel::None;
        } else {
            log(LogLevel::Debug,
                "\nLog level default to 'Info' due to invalid env variable\n");
            currentLogLevel = LogLevel::Info;
        }
    } else {
        log(LogLevel::Debug,
            "\nNo env variable found. Log level defaulting to 'Info'\n");
        currentLogLevel = LogLevel::Info;
    }
    /* LCOV_EXCL_STOP */
}

Logger &Logger::getGlobalLogger() {
    if (loggerInst == nullptr) {
        loggerInst = new Logger();
    }
    return *loggerInst;
}

void Logger::setLogLevel(LogLevel newLevel) {
    currentLogLevel = newLevel;
}

void Logger::log(LogLevel msgLogLevel, const std::string &logString) {
    if (currentLogLevel >= msgLogLevel) {

        #if defined(P2P_RELAY_UNIT_TEST) || defined(P2P_RELAY_SYS_TEST)
        logOutputFile << logString << std::endl;
        #else
        std::cout << logString << std::endl;
        #endif

        #ifdef P2P_RELAY_UNIT_TEST
        logs += logString + "\n";
        #endif 
        
    }
}

void Logger::error(const std::string &logString) {
    const std::string withPrefix = "ERROR : " + logString;
    log(LogLevel::Error, withPrefix);
}

void Logger::info(const std::string &logString) {
    const std::string withPrefix = "INFO  : " + logString;
    log(LogLevel::Info, withPrefix);
}

void Logger::debug(const std::string &logString) {
    const std::string withPrefix = "DEBUG : " + logString;
    log(LogLevel::Debug, withPrefix);
}

#ifdef P2P_RELAY_UNIT_TEST
// LCOV_EXCL_START

const std::string Logger::getLogs() {
    return logs;
}

void Logger::clearLogs() {
    logs = "";
}

// LCOV_EXCL_STOP
#endif
