#include "SessionTable.h"

#include <cstdlib>

#include "Util.h"
#include "Logger.h"

SessionTable::SessionTable() {

    Logger &logger = Logger::getGlobalLogger();
    bool success;
    uint32_t maxNumSessions, sessionIdStart;
    
    /* Determine max number of sessions */
    success = Util::getUint32FromEnv(SESS_TABLE_MAX_NUM_SESSIONS_ENV_VAR,
                                     &maxNumSessions);
    if (success) {
        logger.info("Using value for max number of sessions found in env");
    } else {
        logger.info("Fallback to default value for max number of sessions");
        maxNumSessions = SESS_TABLE_DEFAULT_MAX_NUM_SESSIONS;
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

    logger.info("Creating session table for " + STR(maxNumSessions) +
                " sessions");
    logger.info("with starting id " + STR(sessionIdStart));
}

SessionTable::~SessionTable() {

}
