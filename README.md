# CLI Task Timer (C++)

A command-line timer application built in C++ that allows tracking multiple tasks with persistent storage and flexible matching.

---

## Features

### v1

* Start a timer
* Stop a timer
* Check timer status
* Persistent storage using a file

### v2

* Multiple timers running simultaneously
* Case-insensitive partial name matching
* Support for multi-word task names
* List all active timers
* Improved stop logic:

  * Stops specific timer
  * Handles multiple matches safely

### v2.1 (current)

* Refactored into multi-file project structure
* Separation of concerns:

  * Data (`TimerRecord`)
  * Storage (`TimerState`)
  * Logic (`TimerLogic`)
* Improved code readability and maintainability
* Prepared codebase for future feature expansion

---

## Technologies

* C++
* Standard Library:

  * `vector`
  * `fstream`
  * `chrono`

---

## Usage

```bash
start <task name>
list
status <task name>
stop <task name>
help
```

### Example

```bash
start gym workout
status gym
list
stop gym
```

---

## File Format

Timers are stored in:

```
timer_state.txt
```

Format:

```
taskName|startTime
```

---

## Project Structure

* `main.cpp`
  Entry point and command-line interface handling

* `TimerRecord.h`
  Defines the data structure for a timer

* `TimerState.h / TimerState.cpp`
  Manages timer storage and file persistence

* `TimerLogic.h / TimerLogic.cpp`
  Contains core timer operations (start, stop, status, matching)

---

## Key Learnings

* Designed and used a custom data structure (`TimerRecord`)
* Managed dynamic collections using `std::vector`
* Implemented file persistence with `fstream`
* Built CLI argument parsing logic
* Implemented case-insensitive partial string matching
* Handled edge cases:

  * no timers
  * multiple matches
  * single timer optimization
* Separated responsibilities across multiple files:

  * data (`TimerRecord`)
  * storage (`TimerState`)
  * logic (`TimerLogic`)
* Improved code organization and modular design

---

## Future Improvements

* Improve matching (word-based search instead of substring)
* Add pause / resume functionality
* Improve input validation and error handling
* Add simple UI (e.g. Qt or other framework)

---

## Notes

This project is part of my learning journey in C++ and focuses on building real functionality and understanding core concepts rather than relying on tutorials.
