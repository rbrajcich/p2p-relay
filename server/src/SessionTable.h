#ifndef P2P_RELAY__SESSIONTABLE_H
#define P2P_RELAY__SESSIONTABLE_H

#include <cstdint>
#include <stdexcept>

/* 
 * This error gets thrown by session table if no sessions
 * are available for lease
 */
class OutOfSessionsError : public std::exception {};

/*
 * This error gets thrown by session table if an operation
 * is attempted for a session that's not part of this table
 */
class WrongSessionTableError : public std::exception {};

/* Forward declaration for Session member pointer */
class SessionTable;

/*
 * Represents a session in the session table, including all the info
 * about the socket connection assocated with this session if it's active.
 * Only instantiated by a SessionTable but other consumers may interact
 * with a session once it has been exposed by the SessionTable.
 */
class Session {
 public:

    /* Return the unique id for this session */
    uint32_t getId();

 private:

    /* Private default constructor for use only by friend class SessionTable */
    Session(){}

    /* Unique id of this session table entry */
    uint32_t id;

    /* 
     * True when this session is in the active sessions list.
     * False if the session is available for lease (in the free list)
     */
    bool active;

    /* Next pointer used by SessionTable free or active linked list */
    Session *next;

    /* Prev pointer used by SessionTable active doubly-linked list */
    Session *prev;

    /* Pointer to the SessionTable that owns this session */
    SessionTable *table;

    /* Let SessionTable access private members */
    friend class SessionTable;
};

/*
 * The main table that sessions are stored in. It is created with
 * A constant number of sessions it's capable of handling and range of
 * session ids that it will lease out. These values are able to be specified
 * as environment variables or defaults will be used (see SessionTable.cpp).
 *
 * This table is a data structure specifically designed so that the following
 * operations can be perfomed in constant (O(1)) time complexity:
 * - Retriving a session given its id
 * - Finding an unused session to lease and add to pool of active sessions
 * - Ending a session lease to return it to the pool of free sessions
 * And also allows for iteration through each active session in linear
 * time complexity (based on # active sessions, not total # sessions).
 *
 * To achieve this, the table is an array of Sessions where the session at
 * index 0 has id (sessionIdStart) and each subsequent entry has id 1 higher.
 * Each is also part of ONE of these two lists:
 * - Free sessions (singly-linked queue list, pop from head, queue to tail)
 * - Active sessions (doubly-linked list)
 */
class SessionTable {
 public:

    /* Default constructor - initialize table using env vars or defaults */
    SessionTable();

    /* Destructor - free all memory used by table and its sesions */
    ~SessionTable();

    /* Return the total number of session entries in the table */
    uint32_t getTableSize();

    /* Get the lowest session id leased by the table */
    uint32_t getSessionIdStart();

    /*
     * Create a new session lease and return the session leased.
     * Throws OutOfSessionsError if no sessions available for lease
     */
    Session *createSessionLease();

    /*
     * End the lease of given session and return it to the pool of free
     * sessions. If the session is not leased, nothing is done but no
     * error is thrown. If the id is out of range of the table, throws
     * std::out_of_range.
     */
    void endSessionLease(Session *sess);

 private:

    /* The array of Sessions- allocated when table is constructed */
    Session *sessions;

    /* The size of the 'sessions' array; the max number of leasable sessions */
    uint32_t tableSize;

    /* The lowest session id leased by this table (and id of sessions[0]) */
    uint32_t sessionIdStart;

    /* Pointers to the head and tail of the free sessions linked list */
    Session *freeSessionsHead;
    Session *freeSessionsTail;

    /* Pointers to the head and tail of the active sessions linked list */
    Session *activeSessionsHead;
    Session *activeSessionsTail;

    /* Helper to add the given session to the active sessions list */
    void addActiveSession(Session *session);

    /* Helper to remove the given session from the active sessions list */
    void removeActiveSession(Session *session);
};

#endif
