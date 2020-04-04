#ifndef P2P_RELAY__SESSIONTABLE_H
#define P2P_RELAY__SESSIONTABLE_H

#include <cstdint>

class SessionTable {
 public:

   SessionTable();
   ~SessionTable();
   uint32_t getTableSize();
   uint32_t getSessionIdStart();

 private:

   class Session {
    public:
      void setId(uint32_t id);
    private:
      uint32_t id;
   };

   Session *sessions;
   uint32_t tableSize;
   uint32_t sessionIdStart;
};

#endif
