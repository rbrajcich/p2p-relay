#include "Unittest.h"
#include "SessionTable.h"

#include <stdlib.h>

static const char *MAX_SESS_ENV_VAR = "P2P_SERVER_MAX_NUM_SESSIONS";
static const char *SESS_ID_START_ENV_VAR = "P2P_SERVER_SESSION_ID_START";

static const int DEFAULT_TABLE_SIZE = 1000;
static const int DEFAULT_SESS_ID_START = 0;


LOGGED_TEST(SessionTableTests, CreateWithValidEnvironmentParams) {
    setenv(MAX_SESS_ENV_VAR, "25", true);
    setenv(SESS_ID_START_ENV_VAR, "10000", true);
    SessionTable *table = new SessionTable();
    EXPECT_EQ(table->getTableSize(), 25);
    EXPECT_EQ(table->getSessionIdStart(), 10000);
    delete table;
}

LOGGED_TEST(SessionTableTests, CreateWithInvalidEnvironmentParams) {
    setenv(MAX_SESS_ENV_VAR, "word24", true);
    setenv(SESS_ID_START_ENV_VAR, "not_a_number", true);
    SessionTable *table = new SessionTable();
    EXPECT_EQ(table->getTableSize(), DEFAULT_TABLE_SIZE);
    EXPECT_EQ(table->getSessionIdStart(), DEFAULT_SESS_ID_START);
    delete table;
}

LOGGED_TEST(SessionTableTests, CreateWithNoEnvironmentParams) {
    unsetenv("P2P_SERVER_MAX_NUM_SESSIONS");
    unsetenv("P2P_SERVER_SESSION_ID_START");
    SessionTable *table = new SessionTable();
    EXPECT_EQ(table->getTableSize(), DEFAULT_TABLE_SIZE);
    EXPECT_EQ(table->getSessionIdStart(), DEFAULT_SESS_ID_START);
    delete table;
}

LOGGED_TEST(SessionTableTests, CreateWithSessionIdOverflow) {
    setenv(MAX_SESS_ENV_VAR, "10", true);
    setenv(SESS_ID_START_ENV_VAR, "4294967290", true);

    try {
        SessionTable *table = new SessionTable();
        FAIL() << "Expected std::overflow_error";
    }
    catch(std::overflow_error const & err) { }
    catch(...) {
        FAIL() << "Expected std::overflow_error";
    }
}