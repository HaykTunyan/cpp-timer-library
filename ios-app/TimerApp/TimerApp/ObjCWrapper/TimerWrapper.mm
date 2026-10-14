

#import "TimerWrapper.h"

#include "timer.h"


@implementation TimerWrapper


+ (int)start {
    return ::start();
}


+ (int)stop {
    return ::stop();
}


@end
