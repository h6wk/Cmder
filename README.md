# Cmder

This will be an async task executor

Details later.

# Build
This is a small project. Visual Studio Code's task.json will be used to build the project.

## Windows
Install MSYS2. Follow https://code.visualstudio.com/docs/cpp/config-mingw to setup the developer environment.

## Linux
...


```plantuml

@startuml

class Agent {
        #Agent()
        +~Agent()
        +{static} create() : SharedPtr
        +doTask(Mode mode, const std::string& task) : std::string {query}
}

class Server {
        +Server()
        +~Server()
        -mStatus : Status
        -mConditionVariable : std::condition_variable
        -mMutex : std::mutex
        -mThreadPtr : std::unique_ptr<std::thread>
        -run() : void
        +start() : void
        +stop() : void
}


enum Agent::Mode {
        Async
        Blocking
}


enum Server::Status {
        Init
        Run
        Start
        Stop
}


class scoped_logger {
        +scoped_logger(const std::string& className, const char* funcName, unsigned line)
        +~scoped_logger()
        -_ss : std::stringstream
        +stream() : std::stringstream&
}

.Agent +-- .Agent::Mode

.Server +-- .Server::Status

@enduml
```
