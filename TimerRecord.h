#ifndef TIMERRECORD_H
#define TIMERRECORD_H

#include <string>

// Represents one active timer stored in memory and in the timer_state.txt file

struct TimerRecord
{
    std::string taskName;
    long long startTime;
};


#endif // TIMERRECORD_H
