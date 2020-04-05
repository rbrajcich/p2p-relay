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
    uint32_t id;
    bool idsLeased[10];

    for (i = 0; i < 10; i++) {
        idsLeased[i] = false;
    }

    for (i = 0; i < 10; i++) {
        id = table->createSessionLease();
        EXPECT_TRUE(id >= 0 && id < 10);
        EXPECT_FALSE(idsLeased[id]);
        idsLeased[id] = true;
    }

    /* We want this to throw an error */
    try {
        id = table->createSessionLease();
    } catch(OutOfSessionsError &err) {
        delete table;
        return;
    }

    FAIL() << "Expected OutOfSessionsError";
}

LOGGED_TEST(SessionTableTests, FreeOutOfBoundsId) {

    SessionTable *table = createSessTableWith("10", "15");

    /* We want this to throw an error */
    try {
        table->endSessionLease(14);
        FAIL() << "Expected std::out_of_range";
    } catch(std::out_of_range &err) {

    } catch (...) {
        FAIL() << "Expected std::out_of_range";
    }

        /* We want this to throw an error */
    try {
        table->endSessionLease(25);
        FAIL() << "Expected std::out_of_range";
    } catch(std::out_of_range &err) {

    } catch (...) {
        FAIL() << "Expected std::out_of_range";
    }

    delete table;
}

LOGGED_TEST(SessionTableTests, LeaseAndLeaseAgain) {
    
    SessionTable *table = createSessTableWith("5", "0");
    int i;
    uint32_t id, id2;

    /* Lease all ids */
    table->createSessionLease();
    id = table->createSessionLease();
    id2 = table->createSessionLease();
    table->createSessionLease();
    table->createSessionLease();

    /* Make sure they re-queue in order to be leased again */
    table->endSessionLease(id2);
    table->endSessionLease(id);
    EXPECT_EQ(table->createSessionLease(), id2);
    EXPECT_EQ(table->createSessionLease(), id);

    /* End all leased ids */
    for (i = 0; i < 5; i++) {
        table->endSessionLease(i);
    }

    /* Make sure nothing bad happens if we end an already-freed lease */
    table->endSessionLease(id);
}