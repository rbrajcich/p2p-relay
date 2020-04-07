#include "Timer.h"

Timer *Timer::activeTimersList = nullptr;
Timer *Timer::activeTimersNext = nullptr;

Timer::Timer() {
    isActive = false;
}

void Timer::setCallbackFunction(void (*func)(void)) {
    callback = func;
}

void Timer::setDuration(long millis) {
    timeRemaining = millis;
}

void Timer::start() {
    if (!isActive) {
        activeTimersListAdd(this);
        isActive = true;
    }
}

void Timer::stop() {
    if (isActive) {
        activeTimersListRemove(this);
        isActive = false;
    }
}

void Timer::poll(long elapsedMillis) {
    if (elapsedMillis >= timeRemaining) {
        timeRemaining = 0;
        activeTimersListRemove(this);
        isActive = false;
        if (callback != nullptr) {
            callback();
        }
    } else {
        timeRemaining -= elapsedMillis;
    }
}

void Timer::activeTimersListAdd(Timer *timer) {

    timer->next = activeTimersList;
    timer->prev = nullptr;

    if (activeTimersList != nullptr) {
        activeTimersList->prev = timer;
    }

    activeTimersList = timer;
}

void Timer::activeTimersListRemove(Timer *timer) {

    if (timer->next != nullptr) {
        timer->next->prev = timer->prev;
    }

    if (timer->prev != nullptr) {
        timer->prev->next = timer->next;
    } else {
        activeTimersList = timer->next;
    }

    if (timer == activeTimersNext) {
        activeTimersNext = timer->next;
    }

}

void Timer::pollTimers(long elapsedMillis) {

    Timer *curTimer;
    curTimer = activeTimersList;

    while (curTimer != nullptr) {
        activeTimersNext = curTimer->next;
        curTimer->poll(elapsedMillis);
        curTimer = activeTimersNext;
    }    

}