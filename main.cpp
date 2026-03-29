#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdio>

const std::string TIMER_FILE = "timer_state.txt";

class TimerState
{

private:
    std::string fileName;

public:
    TimerState (const std::string& file)
    : fileName(file)
    {
    }

    bool exists() const;
    void save(const std::string& taskName, long long startTime);
    bool load(std::string& taskName, long long& startTime) const;
    void clear();

};

bool TimerState::exists() const
{
    std::ifstream file(fileName);
    return file.good();
}

void TimerState::save(const std::string& taskName, long long startTime )
{
    std::ofstream file(fileName);
    file << taskName << "\n";
    file << startTime << "\n";
}

bool TimerState::load(std::string& taskName, long long& startTime) const
{
    std::ifstream file(fileName);

    if (!file)
    {
        return false;
    }

    return static_cast<bool>(file >> taskName >> startTime);
}

void TimerState::clear()
{
    std::remove(fileName.c_str());

}

void printElapsed(const std::string& taskName, long long startTime)
{
    auto now = std::chrono::steady_clock::now();
    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now.time_since_epoch()
                     ).count();

    auto elapsed = nowMs - startTime;
    auto seconds = elapsed / 1000;

    std::cout << "Task: " << taskName << "\n";
    std::cout << "Elapsed time: " << seconds << " seconds\n";
}

void printHelp()
{
    std::cout<<"Timer CLI\n\n"
                 "Commands: \n"
                 "   start <task> \n"
                 "   stop \n"
                 "   status \n"
                 "   help \n";
}

void startTimer(TimerState& state, int argc, char *argv[])
{


    if (argc <3)
    {
        std::cout << "Please provide task name\n";
        return;
    }
    if (state.exists())
    {
        std::cout<<"Timer already running \n";
        return;
    }

    std::string taskName = argv[2];

    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()
                  ).count();

    state.save(taskName,ms);

    std::cout<<"Timer started: " << taskName << "\n";

}

void statusTimer(TimerState& state)
{
    std::string taskName;
    long long startTime;

    if(!state.load(taskName, startTime))
    {
        std::cout << "No active timer \n";
        return;
    }

    printElapsed(taskName,startTime);
}

void stopTimer(TimerState& state)
{
    std::string taskName;
    long long startTime;

    if (!state.load(taskName,startTime))
    {
        std::cout << "No active timer \n";
        return;
    }

    printElapsed(taskName,startTime);
    state.clear();
    std::cout << "Timer stopped \n";
}

int main(int argc, char *argv[])
{
    TimerState state(TIMER_FILE);

    if (argc <2)
    {
        printHelp();
        return 0;
    }

    std::string command = argv[1];

    if (command == "start")
    {
        startTimer(state,argc,argv);
    }

    else if (command == "status")
    {
        statusTimer(state);
    }

    else if (command == "stop")
    {
        stopTimer(state);
    }

    else if (command == "help")
    {
        printHelp();
    }
    else
    {
        std::cout<<"Unknown command";
    }
}

