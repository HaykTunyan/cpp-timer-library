

#import "TimerWrapper.h"

#include "timer.h"

// TimerWrapper.mm 
@implementation TimerWrapper

// Wrap the C functions in Objective-C methods

+ (int)start {
    return ::start();
}


+ (int)stop {
    return ::stop();
}


@end
