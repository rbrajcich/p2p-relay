#include "Logger.h"

#include <iostream>

Logger *Logger::loggerInst = nullptr;

#if defined(P2P_RELAY_UNIT_TEST) || defined(P2P_RELAY_SYS_TEST)
std::ofstream Logger::logOutputFile;
#endif

Logger::Logger() {
    currentLogLevel = LogLevel::Info;

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

const LogLevel Logger::getLogLevel() {
    return currentLogLevel;
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

void Logger::destroyLogger() {
    if (loggerInst != nullptr) {
        delete loggerInst;
        loggerInst = nullptr;
    }
}

// LCOV_EXCL_STOP
#endif
