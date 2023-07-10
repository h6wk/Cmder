# Cmder

This will be an async task executor

Details later.

# Build
This is a small project. Visual Studio Code's task.json will be used to build the project.

Extensions used:
- Auto Header Plus v1.3.0 (Auto add header to a new file or existing file)
- C/C++ v1.16.3 (C/C++ IntelliSense, debugging, and code browsing)
- C/C++ Build Task v0.0.1 (Visual Studio Code task provider for compiling C/C++ project)
- C/C++ Extension Pack v1.3.0 (Popular extensions for C++ development in Visual Studio Code)
- C/C++ Themes v2.0.0 (UI Themes for C/C++ extension)
- ~~CMake v0.0.17 (CMake langage support for Visual Studio Code)~~ (FUTURE MUSIC)
- ~~CMake Tools v1.14.34 (Extended CMake support in Visual Studio Code)~~  (FUTURE MUSIC)
- Git History v0.6.20 (View git log, file history, compare branches or commits)
- Markdown Preview Github Styling v2.0.2 (Changes VS Code's built-in markdown preview to match Github's style)
- PlantUML v2.17.5 (Rich PlantUML support for Visual Studio Code)
- Svg Preview v2.8.3 (Preview for Svg files)


## Windows
Install MSYS2. Follow https://code.visualstudio.com/docs/cpp/config-mingw to setup the developer environment.

## Linux
...

# Design

## Class diagram

Using hpp2plantuml. Installation:

**$ pip install hpp2plantuml**

Generate PlantUML:

**$ hpp2plantuml -i "*.hpp" -i ".\tests\*.hpp" -o .\diagrams\classdiagram.puml**



![classdiag](diagrams/classdiagram.svg "Class diagram")

## Sequence diagrams

There are three basically different setups between the user and the agent:
* **Blocking the user's call with an active callback channel**: The caller thread is blocked and released once the result is to deliver. The notification channel is active the server's notifications are forwarded to the clients.
* **Blocking the user's call with an active callback channel**: The same as above, but the notifications are not forwarded to the client.
* **Async response**: In this case it is a MUST to have a callback registered. The caller thread returns immediately to the client, the only way to pass back the result is the callback's notification channel.

### Blocking the client's call with active notification channel
![seq_diagram](diagrams/seq_user_agent_blocking_with_callback.svg "Blocking call")

### Blocking the client's call without callback
![seq_diagram](diagrams/seq_user_agent_blocking_no_callback.svg "Blocking call no callback")

### Async command execution
...