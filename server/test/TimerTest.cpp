#include "Unittest.h"
#include "Timer.h"

static Timer *globalTimer;

static bool callback1Called;
static bool callback2Called;
static bool callback3Called;
static bool callback4Called;
static bool callback5Called;

static void callbackFunction1() {
    callback1Called = true;
}

static void callbackFunction2() {
    callback2Called = true;

}

static void callbackFunction3() {
    callback3Called = true;
    delete globalTimer;
}

static void callbackFunction4() {
    callback4Called = true;
}

static void callbackFunction5() {
    callback5Called = true;
}

LOGGED_TEST(TimerTests, BasicCallback) {

    /* 
     * Setting a timer, make sure it doesn't count time
     * before the start() function gets called
     */
    Timer *timer = new Timer(1000, callbackFunction1);
    callback1Called = false;
    Timer::pollTimers(500);
    timer->start();
    Timer::pollTimers(500);
    EXPECT_FALSE(callback1Called);
    Timer::pollTimers(500);
    EXPECT_TRUE(callback1Called);

    /* Re-setting timer and make sure all still works */
    callback1Called = false;
    Timer::pollTimers(1000);
    EXPECT_FALSE(callback1Called);
    timer->setDuration(2000);
    Timer::pollTimers(1500);
    EXPECT_FALSE(callback1Called);
    timer->start();
    Timer::pollTimers(1500);
    EXPECT_FALSE(callback1Called);
    Timer::pollTimers(1500);
    EXPECT_TRUE(callback1Called);

    /* Try setCallbackFunction() with nullptr for no callback */
    callback1Called = false;
    timer->setDuration(1000);
    timer->setCallbackFunction(nullptr);
    timer->start();
    Timer::pollTimers(1000);
    EXPECT_FALSE(callback1Called);

    delete timer;
}

LOGGED_TEST(TimerTests, StopEarly) {
    
    /* Stop using stop() function before timer expires */
    Timer *timer = new Timer();
    callback1Called = false;
    timer->setCallbackFunction(callbackFunction1);
    timer->setDuration(1000);
    timer->start();
    Timer::pollTimers(900);
    timer->stop();
    Timer::pollTimers(500);
    EXPECT_FALSE(callback1Called);

    /* Stop by deleting timer */
    timer->setDuration(1000);
    timer->start();
    Timer::pollTimers(900);
    delete timer;
    Timer::pollTimers(500);
    EXPECT_FALSE(callback1Called);
}

LOGGED_TEST(TimerTests, MultipleTimers) {

    Timer *timer1 = new Timer(3000, callbackFunction1);
    Timer *timer2 = new Timer(1000, callbackFunction2);
    Timer *timer3 = new Timer(3000, callbackFunction3);
    globalTimer = new Timer(4000, callbackFunction4);
    Timer *timer5 = new Timer(2000, callbackFunction5);

    callback1Called = false;
    callback2Called = false;
    callback3Called = false;
    callback4Called = false;
    callback5Called = false;

    timer5->start();
    globalTimer->start();
    timer3->start();
    timer2->start();
    timer1->start();

    Timer::pollTimers(1000);
    EXPECT_FALSE(callback1Called);
    EXPECT_TRUE(callback2Called);
    EXPECT_FALSE(callback3Called);
    EXPECT_FALSE(callback4Called);
    EXPECT_FALSE(callback5Called);
    callback2Called = false;

    Timer::pollTimers(1000);
    EXPECT_FALSE(callback1Called);
    EXPECT_FALSE(callback2Called);
    EXPECT_FALSE(callback3Called);
    EXPECT_FALSE(callback4Called);
    EXPECT_TRUE(callback5Called);
    callback5Called = false;

    Timer::pollTimers(1000);
    EXPECT_TRUE(callback1Called);
    EXPECT_FALSE(callback2Called);
    EXPECT_TRUE(callback3Called);
    EXPECT_FALSE(callback4Called);
    EXPECT_FALSE(callback5Called);
    callback1Called = false;
    callback3Called = false;

    Timer::pollTimers(5000);
    EXPECT_FALSE(callback1Called);
    EXPECT_FALSE(callback2Called);
    EXPECT_FALSE(callback3Called);
    EXPECT_FALSE(callback4Called);
    EXPECT_FALSE(callback5Called);

    delete timer1;
    delete timer2;
    delete timer3;
    delete timer5;
}