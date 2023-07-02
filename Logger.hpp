#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

inline std::string className(const std::string& prettyFunction)
{
  size_t colons = prettyFunction.rfind("::");
  if (colons == std::string::npos) { 
    return "";
  }
  size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
  size_t end = colons - begin;

  return prettyFunction.substr(begin,end);
}

struct scoped_logger
{
  scoped_logger(const std::string& className, const char* funcName, unsigned line)
    : _ss()
  {
    char name[8];
    pthread_getname_np(pthread_self(), &name[0], sizeof(name));
    _ss << "[" << name << "," << getpid() << "/" << std::this_thread::get_id() 
        << "] " << className << "::" << funcName << "(" << line << "): ";
  }

  std::stringstream& stream(){ return _ss; }
  ~scoped_logger(){ std::cout << "\n" << _ss.str(); }
private:
  std::stringstream _ss;
};

#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)

#define LOG(What) \
  scoped_logger(__CLASS_NAME__, __FUNCTION__, __LINE__).stream() << What


#endif // LOGGER_H_INCLUDED
