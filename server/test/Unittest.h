#ifndef P2P_RELAY__UNITTEST_H
#define P2P_RELAY__UNITTEST_H

#include "gtest/gtest.h"
#include "Logger.h"

#define LOGGED_TEST(s, t)                                                       \
    static void p2p_relay_logged_test_body_ ## s ## _ ## t();                   \
    TEST(s, t) {                                                                \
        Logger &l = Logger::getGlobalLogger();                             \
        l.setLogLevel(LogLevel::Debug);                                    \
        l.debug(std::string("--- Start Test ") + #s + "." + #t + " ---");   \
        p2p_relay_logged_test_body_ ## s ## _ ## t();                           \
        l.setLogLevel(LogLevel::Debug);                                    \
        l.debug(std::string("--- End Test ") + #s + "." + #t + " ---\n");   \
    }                                                                           \
    static void p2p_relay_logged_test_body_ ## s ## _ ## t()

#endif
