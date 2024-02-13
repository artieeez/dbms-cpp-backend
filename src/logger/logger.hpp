#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

// enum log types
enum LogType {
  INFO,
  SCOPE,
  ERROR
};

class Logger {
  private:
    std::string _filePath;
    std::fstream _file;
    std::vector<std::string> _scope;

    void _log(const std::string &message, const LogType &logType);

  public:
    Logger(const std::string &filePath);

    ~Logger();

    void log(const std::string &message, bool error = false);

    void pushScope(const std::string &scope);

    void popScope();
};

#endif