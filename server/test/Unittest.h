#ifndef P2P_RELAY__UNITTEST_H
#define P2P_RELAY__UNITTEST_H

#include "gtest/gtest.h"
#include "Logger.h"

#define LOGGED_TEST(s, t)                                                       \
    static void p2p_relay_logged_test_body_ ## s ## _ ## t();                   \
    TEST(s, t) {                                                                \
        Logger &logger = Logger::getGlobalLogger();                             \
        logger.setLogLevel(LogLevel::Info);                                     \
        logger.info(std::string("--- Start Test ") + #s + "." + #t + " ---");   \
        p2p_relay_logged_test_body_ ## s ## _ ## t();                           \
        logger.setLogLevel(LogLevel::Info);                                     \
        logger.info(std::string("--- End Test ") + #s + "." + #t + " ---\n");   \
    }                                                                           \
    static void p2p_relay_logged_test_body_ ## s ## _ ## t()

#endif
