#ifndef TIMERSTATE_H
#define TIMERSTATE_H

#include <vector>
#include "TimerRecord.h"

class TimerState
{
private:
    std::string fileName;

public:
    TimerState(const std::string& file)
        :fileName (file)
    {

    }
    bool saveAll(const std::vector<TimerRecord>& timers) const;
    std::vector<TimerRecord> loadAll()const;
};

#endif // TIMERSTATE_H
