#include "Unittest.h"

#include <string>

static const std::string err_msg = "ERROR : error log msg\n";
static const std::string inf_msg = "INFO  : info log msg\n";
static const std::string dbg_msg = "DEBUG : debug log msg\n";

static void logMessages(Logger &logger) {
    logger.error("error log msg");
    logger.info("info log msg");
    logger.debug("debug log msg");
}

LOGGED_TEST(LoggerTests, LogLevelNone) {
    Logger &logger = Logger::getGlobalLogger();
    logger.setLogLevel(LogLevel::None);
    logger.clearLogs();
    logMessages(logger);
    EXPECT_EQ(logger.getLogs(), "");
}

LOGGED_TEST(LoggerTests, LogLevelError) {
    Logger &logger = Logger::getGlobalLogger();
    logger.setLogLevel(LogLevel::Error);
    logger.clearLogs();
    logMessages(logger);
    EXPECT_EQ(logger.getLogs(), err_msg);
}

LOGGED_TEST(LoggerTests, LogLevelInfo) {
    Logger &logger = Logger::getGlobalLogger();
    logger.setLogLevel(LogLevel::Info);
    logger.clearLogs();
    logMessages(logger);
    EXPECT_EQ(logger.getLogs(), err_msg + inf_msg);
}

LOGGED_TEST(LoggerTests, LogLevelDebug) {
    Logger &logger = Logger::getGlobalLogger();
    logger.setLogLevel(LogLevel::Debug);
    logger.clearLogs();
    logMessages(logger);
    EXPECT_EQ(logger.getLogs(), err_msg + inf_msg + dbg_msg);
}

LOGGED_TEST(LoggerTests, LogMacroSTR) {
    Logger &logger = Logger::getGlobalLogger();
    logger.setLogLevel(LogLevel::Debug);
    logger.clearLogs();
    logger.debug("The number " + STR(50));
    EXPECT_EQ(logger.getLogs(), "DEBUG : The number 50\n");
}
