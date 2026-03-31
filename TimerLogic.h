#ifndef TIMERLOGIC_H
#define TIMERLOGIC_H

#include <vector>
#include "TimerRecord.h"
#include "TimerState.h"

bool timerExists(const std::string& taskName, const std::vector<TimerRecord>& timers);

std::string getTaskName(int argc, char* argv[]);

void startTimer(TimerState& state, int argc, char* argv[]);

long calcElapsedSeconds (const long long startTime, const long long nowMs);

std::string toLowerCase (std::string taskName);

std::vector<TimerRecord> findMatches(const std::vector<TimerRecord>& timers,std::string taskName);

void printTime(TimerState& state,int argc,char* argv[]);

void stop(TimerState& state, int argc, char* argv[]);

void printList(TimerState& state);

void printHelp();




#endif // TIMERLOGIC_H
