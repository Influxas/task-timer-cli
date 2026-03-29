#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdio>
#include <vector>

const std::string TIMER_FILE = "timer_state.txt";

// Represents one active timer stored in memory and in the timer_state.txt file
struct TimerRecord
{
    std::string taskName;
    long long startTime;
};

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

bool timerExists(const std::string& taskName, const std::vector<TimerRecord>& timers)
{
    for (const auto& item : timers)
    {
        if (item.taskName == taskName)
        {
            return true;
        }
    }
    return false;
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

// Joins all comand-line arguments from argv[2] onward into one task name.
std::string getTaskName(int argc, char* argv[])
{

    std::string getTaskName;
    bool first = true;

    for (int i = 2; i < argc ; i++ )
    {
        if (!first)
        {
            getTaskName.append(" ");
            getTaskName.append(argv[i]);
        }
        else
        {
            getTaskName.append(argv[i]);
            first = false;
        }
    }
    return getTaskName;
}


void startTimer(TimerState& state, int argc, char* argv[])
{
    if (argc<3)
    {
        std::cout << "Enter task name \n";
        return ;
    }

    std::string taskName = getTaskName(argc, argv);

    std::vector<TimerRecord> timers = state.loadAll();

    if (timerExists(taskName,timers))
    {
        std::cout << "Timer with name: " << taskName << " already running \n";
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()
                  ).count();

    TimerRecord r;
    r.taskName = taskName;
    r.startTime = ms;
    timers.push_back(r);

    if (!state.saveAll(timers))
    {
        std::cout << "Timer failed to start \n";
        return;
    }
    std::cout << "Task: " << taskName << " started \n";
}

long calcElapsedSeconds (const long long startTime, const long long nowMs)
{
    auto elapsed = nowMs - startTime;
    auto seconds = elapsed / 1000;
    return seconds;
}

std::string toLowerCase (std::string taskName)
{
    for (auto& c : taskName)
    {
        c = std::tolower((unsigned char)c);
    }
    return taskName;
}

// Performs case-insensitive partial matching against stored timer names.
// Returns all matching timers.
std::vector<TimerRecord> findMatches(const std::vector<TimerRecord>& timers,std::string taskName)
{
    std::vector <TimerRecord> matches;

    auto normalizedQuery = toLowerCase(taskName);

    for (const auto& item : timers)
    {

        auto normalizedStored = toLowerCase(item.taskName);

        if (normalizedStored.find(normalizedQuery) != std::string::npos)
        {
            matches.push_back(item);
        }
    }
    return matches;
}

// Prints timer status.
// If only one timer exists, it is printed directly.
// If multiple timers exists, a task name must be provided.
void printTime(TimerState& state,int argc,char* argv[])
{

    auto timers = state.loadAll();

    auto now = std::chrono::steady_clock::now();
    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now.time_since_epoch()
                                                  ).count();

    if (timers.empty())
    {
        std::cout << "No active timers \n ";
        return;
    }
    else if (timers.size() == 1)
    {
        std::cout << "Task: " << timers[0].taskName << " elapsed time: " << calcElapsedSeconds(timers[0].startTime, nowMs) << " seconds \n";
        return;
    }
    else if (argc <3)
    {
        std::cout << "Enter task name \n ";
        return;
    }

    std::string taskName = getTaskName(argc, argv);

    auto matches = findMatches(timers,taskName);

    if (matches.empty())
    {
        std::cout << "not found \n";
    }
    else if (matches.size() == 1)
    {
        std::cout << "Task: " << matches[0].taskName << " elapsed time: " << calcElapsedSeconds(matches[0].startTime, nowMs) << " seconds \n";
    }
    else if (matches.size() > 1)
    {
        std::cout << "More than one active timer with this name please specify \n";
        for (const auto& item: matches)
        {
            std::cout << item.taskName << " \n";
        }
    }
}

// Stops a timer.
// If only one timer exists, it is stopped directly.
// If multiple timers exists, a task name must be provided.
void stop(TimerState& state, int argc, char* argv[])
{
    auto timers = state.loadAll();

    auto now = std::chrono::steady_clock::now();
    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
                     ).count();

    if (timers.empty())
    {
        std::cout << "no active timer \n";
        return;
    }
    else if (timers.size() == 1)
    {
        std::cout << "Task: " << timers[0].taskName << " elapsed time: " << calcElapsedSeconds(timers[0].startTime, nowMs) << " seconds \n";
        timers.erase(timers.begin() + 0);
        state.saveAll(timers);
        std::cout << "Timer has been stopped \n";
        return;
    }
    else if ( argc <3)
    {
        std::cout << "enter task name \n";
        return;
    }

    std::string taskName = getTaskName(argc,argv);

    auto matches = findMatches(timers,taskName);

    if (matches.empty())
    {
        std::cout << "not found \n";
        return;
    }
    else if (matches.size() == 1)
    {
        std::cout << "Task: " << matches[0].taskName << " elapsed time: " << calcElapsedSeconds(matches[0].startTime, nowMs) << " seconds \n";

        // Remove the exact matched timer from the main collection.
        // Compare both taskName and startTime to identify the correct record.
        for (int i=0; i<timers.size(); i++)
        {
            if (timers[i].taskName == matches[0].taskName && timers[i].startTime == matches[0].startTime)
            {
                timers.erase(timers.begin() + i);
                break;
            }
        }

        state.saveAll(timers);

        std::cout << "Timer has been stopped \n";

        return;
    }
    else if (matches.size() >1)
    {
        std::cout << "More than one active timer with this name please specify \n";
        return;
    }
}

void printList(TimerState& state)
{
    auto timers = state.loadAll();

    auto now = std::chrono::steady_clock::now();
    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
                     ).count();
    if (timers.empty())
    {
        std::cout << "No active timers \n";
        return;
    }

    for (const auto& item : timers)
    {

        std::cout << "Timer: " << item.taskName << " elapsed time: " << calcElapsedSeconds(item.startTime, nowMs) << " seconds \n";
    }
}

void printHelp()
{
    std::cout <<"CLI timer \n"
                 "   Commands: \n"
                 "   start <task> \n"
                 "   list \n"
                 "   status <task> \n"
                 "   stop \n"
                 "   help \n";
}


int main (int argc, char* argv[])
{
    TimerState state(TIMER_FILE);

    if (argc <2)
    {
        std::cout << "Please enter command or refer to help \n";
        return 0;
    }

    std::string command = argv[1];

    if (command == "start")
    {
        startTimer(state,argc,argv);
    }
    else if (command == "list")
    {
        printList(state);
    }
    else if (command == "status")
    {
        printTime(state,argc,argv);
    }
    else if (command == "stop")
    {
        stop(state, argc,argv);
    }
    else if (command == "help")
    {
        printHelp();
    }
    else
    {
        std::cout <<"Unknown command \n";
    }
}
