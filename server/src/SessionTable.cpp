#include "SessionTable.h"

#include <cstdlib>
#include <stdexcept>

#include "Util.h"
#include "Logger.h"

/* 
 * The environment variable that can be set to override the
 * below default value SESS_TABLE_DEFAULT_MAX_NUM_SESSIONS
 */
#define SESS_TABLE_MAX_NUM_SESSIONS_ENV_VAR "P2P_SERVER_MAX_NUM_SESSIONS"

/* 
 * The environment variable that can be set to override the
 * below default value SESS_TABLE_DEFAULT_ID_START
 */
#define SESS_TABLE_ID_START_ENV_VAR "P2P_SERVER_SESSION_ID_START"

/* 
 * Default value for the session table size (max num of sessions).
 */
#define SESS_TABLE_DEFAULT_MAX_NUM_SESSIONS 1000

/*
 * Default value for start of session ids in table (allotted session ids will
 * be consecutive values counting upwards from this value. The highest session
 * id (this value + max num sessions) may not surpass MAX(UINT32)
 */
#define SESS_TABLE_DEFAULT_ID_START 0

void SessionTable::Session::setId(uint32_t id) {
    this->id = id;
}

SessionTable::SessionTable() {

    Logger &logger = Logger::getGlobalLogger();
    bool success;
    int i, curId;
    
    /* Determine max number of sessions */
    success = Util::getUint32FromEnv(SESS_TABLE_MAX_NUM_SESSIONS_ENV_VAR,
                                     &tableSize);
    if (success) {
        logger.info("Using value for max number of sessions found in env");
    } else {
        logger.info("Fallback to default value for max number of sessions");
        tableSize = SESS_TABLE_DEFAULT_MAX_NUM_SESSIONS;
    }

    /* Determine session id starting value */
    success = Util::getUint32FromEnv(SESS_TABLE_ID_START_ENV_VAR,
                                     &sessionIdStart);
    if (success) {
        logger.info("Using value for session id start found in env");
    } else {
        logger.info("Fallback to default value for session id start");
        sessionIdStart = SESS_TABLE_DEFAULT_ID_START;
    }

    if ((sessionIdStart + tableSize) < sessionIdStart) {
        /* Overflowed uint32_t */
        logger.error("Creating " + STR(tableSize) +
                     " sessions with starting id " +
                     STR(sessionIdStart) + " overflows uint32_t id field");
        throw std::overflow_error("Last session id > max value of uint32_t");
    }

    logger.info("Creating session table for " + STR(tableSize) +
                " sessions");
    logger.info("with starting id " + STR(sessionIdStart));

    /* Create the actual table */
    sessions = new Session[tableSize];

    curId = sessionIdStart;
    for (i = 0; i < tableSize; i++) {
        sessions[i].setId(curId++);
    }

}

SessionTable::~SessionTable() {
    delete[] sessions;
    sessions = nullptr;
}

uint32_t SessionTable::getTableSize() {
    return tableSize;
}

uint32_t SessionTable::getSessionIdStart() {
    return sessionIdStart;
}
