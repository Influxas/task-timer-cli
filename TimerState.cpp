#include "TimerState.h"
#include <fstream>

// Loads all timers from file.
// File format per line: taskName|startTime
// Invalid lines are skipped.

std::vector<TimerRecord> TimerState::loadAll()const
{
    std::vector<TimerRecord> t;

    std::ifstream file (fileName);

    std::string line;

    while (std::getline(file, line))
    {
        size_t pos = line.find('|');
        if( pos == std::string::npos)
        {
            continue;
        }
        auto taskName = line.substr(0, pos);
        auto startTimeText = line.substr(pos +1);

        try
        {
            long long startTime = stoll(startTimeText);
            TimerRecord r;
            r.taskName = taskName;
            r.startTime = startTime;
            t.push_back(r);
        }
        catch (const std::exception&)
        {
            continue;
        }
    }
    return t;
}

// Rewrites timer file using the current in-memory timer collection.

bool TimerState::saveAll(const std::vector<TimerRecord>& timers) const
{
    std::ofstream file(fileName);

    for (const auto& item : timers)
    {
        file << item.taskName << "|" << item.startTime << "\n";
    }

    return file.good();
}