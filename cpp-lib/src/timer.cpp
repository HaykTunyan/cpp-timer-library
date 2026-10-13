
#include "timer_api.h"

#include <chrono>
#include <memory>


// Timer class to measure elapsed time.
class Timer {
    
    public:
    
    // Constructor to initialize the timer
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

// 
static std::unique_ptr<Timer> timer;

// Function to Start Timer
int start() {
  
    if (timer == nullptr) {
        timer = std::make_unique<Timer>();
    }
    return 0;
}

// Function to Stop Timer
int stop() {

    if (timer == nullptr) {
        return -1;
    }

    int seconds = timer->getSeconds();
    
    timer.reset();

    return seconds;
}
       
