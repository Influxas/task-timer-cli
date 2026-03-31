#include <iostream>
#include <cstdio>
#include "TimerState.h"
#include "TimerLogic.h"

const std::string TIMER_FILE = "timer_state.txt";

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
