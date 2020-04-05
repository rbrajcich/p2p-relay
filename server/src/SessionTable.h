#ifndef P2P_RELAY__SESSIONTABLE_H
#define P2P_RELAY__SESSIONTABLE_H

#include <cstdint>
#include <stdexcept>

class OutOfSessionsError : public std::exception {

};

class SessionTable {
 public:

    SessionTable();
    ~SessionTable();
    uint32_t getTableSize();
    uint32_t getSessionIdStart();
    uint32_t createSessionLease();
    void endSessionLease(uint32_t id);

 private:

    typedef struct _Session{
        uint32_t id;
        bool active;
        struct _Session *next;
        struct _Session *prev;
    } Session;

    Session *sessions;
    uint32_t tableSize;
    uint32_t sessionIdStart;
    Session *freeSessionsHead;
    Session *freeSessionsTail;
    Session *activeSessionsHead;
    Session *activeSessionsTail;
    void addActiveSession(Session *session);
    void removeActiveSession(Session *session);
};

#endif
