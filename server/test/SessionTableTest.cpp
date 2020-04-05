#include "Unittest.h"
#include "SessionTable.h"

#include <stdlib.h>

static const char *MAX_SESS_ENV_VAR = "P2P_SERVER_MAX_NUM_SESSIONS";
static const char *SESS_ID_START_ENV_VAR = "P2P_SERVER_SESSION_ID_START";

static const int DEFAULT_TABLE_SIZE = 1000;
static const int DEFAULT_SESS_ID_START = 0;

static inline SessionTable *createSessTableWith(const char *tableSize,
        const char *startingId) {

    setenv(MAX_SESS_ENV_VAR, tableSize, true);
    setenv(SESS_ID_START_ENV_VAR, startingId, true);
    return new SessionTable();
}

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

    /* We want this to throw an error */
    try {
        SessionTable *table = new SessionTable();
    } catch (std::overflow_error &err) {
        return;
    }
    
    FAIL() << "Expected std::overflow_error";
}

LOGGED_TEST(SessionTableTests, LeaseIdRangeAndLimit) {

    SessionTable *table = createSessTableWith("10", "0");
    int i;
    Session *session;
    bool idsLeased[10];

    for (i = 0; i < 10; i++) {
        idsLeased[i] = false;
    }

    for (i = 0; i < 10; i++) {
        session = table->createSessionLease();
        EXPECT_TRUE(session->getId() >= 0 && session->getId() < 10);
        EXPECT_FALSE(idsLeased[session->getId()]);
        idsLeased[session->getId()] = true;
    }

    /* We want this to throw an error */
    try {
        table->createSessionLease();
    } catch(OutOfSessionsError &err) {
        delete table;
        return;
    }

    FAIL() << "Expected OutOfSessionsError";
}

LOGGED_TEST(SessionTableTests, FreeSessionFromWrongTable) {

    SessionTable *table = createSessTableWith("10", "15");
    SessionTable *table2 = createSessTableWith("10", "15");

    /* We want this to throw an error */
    try {
        table->endSessionLease(table2->createSessionLease());
        FAIL() << "Expected WrongSessionTableError";
    } catch(WrongSessionTableError &err) {

    } catch (...) {
        FAIL() << "Expected WrongSessionTableError";
    }

    delete table;
    delete table2;
}

LOGGED_TEST(SessionTableTests, LeaseAndLeaseAgain) {
    
    SessionTable *table = createSessTableWith("5", "0");
    int i;
    Session *sessions[5];

    /* Lease all sessions */
    for (i = 0; i < 5; i++) {
        sessions[i] = table->createSessionLease();
    }

    /* Make sure they re-queue in order to be leased again */
    table->endSessionLease(sessions[2]);
    table->endSessionLease(sessions[1]);
    EXPECT_EQ(table->createSessionLease(), sessions[2]);
    EXPECT_EQ(table->createSessionLease(), sessions[1]);

    /* End all leases */
    for (i = 0; i < 5; i++) {
        table->endSessionLease(sessions[i]);
    }

    /* Make sure nothing bad happens if we end an already-freed lease */
    table->endSessionLease(sessions[2]);
}