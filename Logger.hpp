#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

inline static std::mutex sLoggerMutex;

/// Example:
///   Receipt Agent::doTask(Agent::Mode mode, Agent::Task task, std::string& result) const
///   Agent::doTask(Mode, Task, std::string&) const::<lambda()>
inline std::string className(const std::string& prettyFunction)
{
  //bool isLambda = (prettyFunction.find("<lambda()>") != std::string::npos);
  size_t openBracket = prettyFunction.find("(");

  size_t colons = prettyFunction.substr(0, openBracket).rfind("::");
  if (colons == std::string::npos) { 
    return "";
  }
  size_t begin = prettyFunction.substr(0, colons).rfind(" ") + 1;
  size_t end = colons - begin;

  std::string result(prettyFunction.substr(begin, end));
  return result;
}

struct scoped_logger
{
  scoped_logger(const std::string& className, const char* funcName, unsigned line)
    : _ss()
  {
    //char name[8];
    //pthread_getname_np(pthread_self(), &name[0], sizeof(name));

    _ss << "[" /*<< name << "," */ << getpid() << "/" << std::this_thread::get_id() 
        << "] " << className << "::" << funcName << "(" << line << "): ";
  }

  std::stringstream& stream(){ return _ss; }
  ~scoped_logger()
  {
    std::lock_guard<std::mutex> guard(sLoggerMutex);
    std::cout << "\n" << _ss.str();
  }

private:
  std::stringstream _ss;
};

#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)

#define LOG(What) \
  scoped_logger(__CLASS_NAME__, __FUNCTION__, __LINE__).stream() << What


#endif // LOGGER_H_INCLUDED
