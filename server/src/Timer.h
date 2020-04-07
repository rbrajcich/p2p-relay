#ifndef P2P_RELAY__TIMER_H
#define P2P_RELAY__TIMER_H

class Timer {
 public:

    long timeRemaining;
    void (*callback)(void);

    Timer();
    Timer(long millis, void (*func)(void));
    ~Timer();

    void setCallbackFunction(void (*func)(void));
    void setDuration(long millis);
    void start();
    void stop();

    static void pollTimers(long elapsedMillis);
 private:

    bool isActive;
    Timer *next;
    Timer *prev;

    void poll(long elapsedMillis);

    static Timer *activeTimersList;
    static Timer *activeTimersNext;
    static void activeTimersListAdd(Timer *timer);
    static void activeTimersListRemove(Timer *timer);
};

#endif
