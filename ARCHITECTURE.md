# CLI Task Timer — Architecture Map

## Files

### main.cpp
Routes CLI commands to TimerLogic functions.

Commands:
- start
- stop
- status
- list
- help

Planned commands:
- pause
- resume

### TimerRecord.h
Defines one timer record.

Current fields:
- taskName
- startTime

Future pause/resume fields:
- elapsedBeforePause
- isPaused

### TimerState.h / TimerState.cpp
Handles file persistence.

Responsibilities:
- loadAll()
- saveAll()

File:
- timer_state.txt

Current format:
taskName|startTime

Future format:
taskName|startTime|elapsedBeforePause|isPaused

### TimerLogic.h / TimerLogic.cpp
Contains timer behaviour.

Functions:
- getTaskName()
- timerExists()
- startTimer()
- stop()
- printTime()
- printList()
- splitWords()
- findMatches()
- selectMatch()
- calcElapsedSeconds()
- formatDuration()

## Current data flow

start:
main.cpp → startTimer() → loadAll() → add TimerRecord → saveAll()

status:
main.cpp → printTime() → loadAll() → findMatches() → calcElapsedSeconds() → formatDuration()

stop:
main.cpp → stop() → loadAll() → findMatches() → optional selectMatch() → erase TimerRecord → saveAll()

list:
main.cpp → printList() → loadAll() → calcElapsedSeconds() → formatDuration()

## Matching rule

findMatches() returns all timers where every query word exists in stored task name.

If multiple matches:
selectMatch() asks user to choose a numbered option.

## Development rule

Before adding a feature:
1. Identify affected files
2. Identify affected functions
3. Update one layer at a time
4. Compile and test after each layer