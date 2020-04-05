#include "SessionTable.h"

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

static Logger &logger = Logger::getGlobalLogger();

SessionTable::SessionTable() {

    bool success;
    int i, curId;
    
    /* Determine max number of sessions */
    success = Util::getUint32FromEnv(SESS_TABLE_MAX_NUM_SESSIONS_ENV_VAR,
                                     &tableSize);
    if (success) {
        logger.debug("Using value for max number of sessions found in env");
    } else {
        logger.debug("Fallback to default value for max number of sessions");
        tableSize = SESS_TABLE_DEFAULT_MAX_NUM_SESSIONS;
    }

    /* Determine session id starting value */
    success = Util::getUint32FromEnv(SESS_TABLE_ID_START_ENV_VAR,
                                     &sessionIdStart);
    if (success) {
        logger.debug("Using value for session id start found in env");
    } else {
        logger.debug("Fallback to default value for session id start");
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
                " sessions with starting id " + STR(sessionIdStart));

    /* Create the actual table */
    sessions = new Session[tableSize];

    /* 
     * Initialize ids and populate in free sessions list,
     * keeping active sessions list empty
     */
    curId = sessionIdStart;
    for (i = 0; i < tableSize - 1; i++) {
        sessions[i].id = curId++;
        sessions[i].active = false;
        sessions[i].prev = nullptr;
        sessions[i].next = &sessions[i+1];
    }
    sessions[tableSize-1].id = curId;
    sessions[tableSize-1].active = false;
    sessions[tableSize-1].prev = nullptr;
    sessions[tableSize-1].next = nullptr;
    freeSessionsHead = &sessions[0];
    freeSessionsTail = &sessions[tableSize-1];
    activeSessionsHead = nullptr;
    activeSessionsTail = nullptr;

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

uint32_t SessionTable::createSessionLease() {

    if (freeSessionsHead == nullptr) {
        throw OutOfSessionsError();
    }

    /* Pop from head of free list and add to active list */
    Session *sess = freeSessionsHead;
    freeSessionsHead = sess->next;
    if (freeSessionsHead == nullptr) {
        freeSessionsTail = nullptr;
    }

    logger.debug("Creating session lease with id " + STR(sess->id));
    addActiveSession(sess);
    sess->active = true;
    return sess->id;
}

void SessionTable::endSessionLease(uint32_t id) {

    uint32_t idx = id - sessionIdStart;

    if (idx >= tableSize) {
        throw std::out_of_range("Session id not in range of table");
    }

    /* If active, move the session to the end of the free list */
    if (sessions[idx].active == true) {
        removeActiveSession(&sessions[idx]);

        logger.debug("Ending lease for session with id " +
                     STR(sessions[idx].id));

        sessions[idx].next = nullptr;
        if (freeSessionsTail == nullptr) {
            freeSessionsHead = &sessions[idx];
        } else {
            freeSessionsTail->next = &sessions[idx];
        }
        freeSessionsTail = &sessions[idx];
        sessions[idx].active = false;
    }

}

void SessionTable::addActiveSession(Session *session) {

    if (activeSessionsTail == nullptr) {
        /* No sessions in list */
        activeSessionsTail = session;
        activeSessionsHead = session;
        session->next = nullptr;
        session->prev = nullptr;
    } else {
        /* Append to end of active sess list */
        activeSessionsTail->next = session;
        session->next = nullptr;
        session->prev = activeSessionsTail;
        activeSessionsTail = session;
    }
}

void SessionTable::removeActiveSession(Session *session) {

    if (session->prev == nullptr) {
        activeSessionsHead = session->next;
    } else {
        session->prev->next = session->next;
    }

    if (session->next == nullptr) {
        activeSessionsTail = session->prev;
    } else {
        session->next->prev = session->prev;
    }
}
