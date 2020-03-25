#ifndef P2P_RELAY__SESSIONTABLE_H
#define P2P_RELAY__SESSIONTABLE_H

#include <cstdint>

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
 * be consecutive values counting upwards from this value and wrapping around
 * to zero if (this value) + (the number of sessions) > (max val of uint32_t).
 */
#define SESS_TABLE_DEFAULT_ID_START 0

class Session {
 public:

 private:
    Session();
    uint32_t id;
};

class SessionTable {
 public:
    SessionTable();
    ~SessionTable();
 private:
    Session *sessions;
};

#endif
