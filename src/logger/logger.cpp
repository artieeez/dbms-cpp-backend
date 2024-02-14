#include "logger.hpp"
#include <filesystem>

Logger::Logger(const std::string &filePath) {
    _filePath1 = "cache/log/" + filePath + "1.log";
    _filePath2 = "cache/log/" + filePath + "2.log";
    _filePath3 = "cache/log/" + filePath + "3.log";
    _filePath5 = "cache/log/" + filePath + "5.log";
    _filePath8 = "cache/log/" + filePath + "8.log";

    if (!std::filesystem::exists("cache")) {
        std::filesystem::create_directory("cache");
        std::filesystem::create_directory("cache/log");
    } else if (!std::filesystem::exists("cache/log")) {
        std::filesystem::create_directory("cache/log");
    }

    _file1 = std::fstream(_filePath1, std::ios::out | std::ios::app);
    _file2 = std::fstream(_filePath2, std::ios::out | std::ios::app);
    _file3 = std::fstream(_filePath3, std::ios::out | std::ios::app);
    _file5 = std::fstream(_filePath5, std::ios::out | std::ios::app);
    _file8 = std::fstream(_filePath8, std::ios::out | std::ios::app);

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

    std::string result = timestamp + indentation;

    switch (logType) {
    case INFO:
        // _file1 << "Log: ";
        break;
    case SCOPE:
        result += "Info: ";
        break;
    case ERROR:
        result += "Error: ";
        break;
    }
    result += message;

    if (scopeSize <= 1) {
        _file1 << result << std::endl;
    }
    if (scopeSize <= 2) {
        _file2 << result << std::endl;
    }
    if (scopeSize <= 3) {
        _file3 << result << std::endl;
    }
    if (scopeSize <= 5) {
        _file5 << result << std::endl;
    }
    _file8 << result << std::endl;
}

void Logger::log(const std::string &message, bool error) {
    _log(message, error ? ERROR : INFO);
}

void Logger::pushScope(const std::string &scope) {
    _scope.push_back(scope);
    _log(scope, SCOPE);
}

void Logger::popScope() {
    if (!_scope.empty()) {
        _scope.pop_back();
    } else {
        _log("Error: No scope to pop", ERROR);
    }
}

Logger mainLogger("main");
