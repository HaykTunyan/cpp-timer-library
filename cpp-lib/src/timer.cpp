
#include "timer.h"

// Include < chrono> for time measurement, < memory> for Std::unique_ptr, and < cstdio> for printing to stdout.
#include <chrono>
#include <memory>
#include <cstdio>


// Timer class to measure elapsed time.
class Timer {
    
    public:
    // Constructor to initialize the timer.
    Timer() {
        startTime = std::chrono::steady_clock::now();
    }

    // Fuction to get the time in secconds since the time was started.
    int getSeconds() {
        auto now = std::chrono::steady_clock::now();
        auto diff = now - startTime;
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(diff);
        return seconds.count();
    }
    
    private:
    std::chrono::steady_clock::time_point startTime;

};

// Static unique pointer to Timer instance.
static std::unique_ptr<Timer> timer;

// Function to Start Timer.
int start() {
  
    if (timer == nullptr) {
        timer = std::make_unique<Timer>();
    }

    // Print message to indicate that the timer has started.
    std::printf("C++: timer started\n");
    std::fflush(stdout);

    return 0;
}

// Function to Stop Timer.
int stop() {

    if (timer == nullptr) {
        return -1;
    }

    int seconds = timer->getSeconds();

    // Print message to indicate that the timer has stoped.
    std::printf("C++: timer stopped, seconds = %d\n", seconds);
    std::fflush(stdout);
    
    timer.reset();

    return seconds;
}
       
