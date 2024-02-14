#include "logger.hpp"
#include <filesystem>

Logger::Logger(const std::string &filePath) {
    _logCount.push_back(0);

    _filePath1 = "cache/log/" + filePath + "1.log";
    _filePath2 = "cache/log/" + filePath + "2.log";

    if (!std::filesystem::exists("cache")) {
        std::filesystem::create_directory("cache");
        std::filesystem::create_directory("cache/log");
    } else if (!std::filesystem::exists("cache/log")) {
        std::filesystem::create_directory("cache/log");
    }

    _file1 = std::fstream(_filePath1, std::ios::out | std::ios::app);
    _file2 = std::fstream(_filePath2, std::ios::out | std::ios::app);

    // Check if the file is open
    if (!_file1.is_open()) {
        std::cerr << "Error opening or creating file: " << _filePath1 << std::endl;
        // Handle the error as needed
    }

    if (!_file2.is_open()) {
        std::cerr << "Error opening or creating file: " << _filePath2 << std::endl;
        // Handle the error as needed
    }

    _file1 << std::endl;
    _file2 << std::endl;
    _file1 << "Log class instantiated at: " << __TIME__ << " " << __DATE__ << std::endl;
    _file2 << "Log class instantiated at: " << __TIME__ << " " << __DATE__ << std::endl;
}

Logger::~Logger() {
    _file1.close();
    _file2.close();
}

void Logger::_log(const std::string &message, const LogType &logType) {
    std::string indentation = "";
    int scopeSize = _scope.size();

    for (int scopeIndex = 0; scopeIndex < scopeSize; scopeIndex++) {
        if (scopeIndex < scopeSize - 1) {
            indentation += "|   ";
        } else if (scopeIndex == scopeSize - 1) {
            indentation += "+---";
        }
    }

    if (logType == SCOPE) {
        indentation += "┌ ";
    } else {
        indentation += "│ ";
    }

    std::string timestamp = __TIME__ + std::string(" ") + __DATE__ + std::string(":");
    _file1 << timestamp << indentation;

    switch (logType) {
    case INFO:
        // _file1 << "Log: ";
        break;
    case SCOPE:
        _file1 << "Info: ";
        break;
    case ERROR:
        _file1 << "Error: ";
        break;
    }
    _file1 << message << std::endl;
}

void Logger::log(const std::string &message, bool error) {
    _log(message, error ? ERROR : INFO);
}

void Logger::pushScope(const std::string &scope) {
    _logCount.push_back(0);
    _scope.push_back(scope);
    _log(scope, SCOPE);
}

void Logger::popScope() {
    if (!_scope.empty()) {
        _logCount.pop_back();
        _scope.pop_back();
    } else {
        _log("Error: No scope to pop", ERROR);
    }
}

Logger mainLogger("main");
