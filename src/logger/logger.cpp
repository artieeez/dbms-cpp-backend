#include "logger.hpp"
#include <filesystem>

Logger::Logger(const std::string &filePath) {
    _filePath = "cache/log/" + filePath + ".log";

    if (!std::filesystem::exists("cache")) {
        std::filesystem::create_directory("cache");
        std::filesystem::create_directory("cache/log");
    } else if (!std::filesystem::exists("cache/log")) {
        std::filesystem::create_directory("cache/log");
    }

    _file = std::fstream(_filePath, std::ios::out | std::ios::app);

    // Check if the file is open
    if (!_file.is_open()) {
        std::cerr << "Error opening or creating file: " << _filePath << std::endl;
        // Handle the error as needed
    }

    _file << std::endl;
    _file << "Log class instantiated at: " << __TIME__ << " " << __DATE__ << std::endl;
}

Logger::~Logger() {
    _file.close();
}

void Logger::_log(const std::string &message, const LogType &logType) {
    std::string indentation = "";
    for (int i = 0; i < _scope.size(); i++) {
        indentation += "    ";
    }

    std::string timestamp = __TIME__ + std::string(" ") + __DATE__ + std::string(":");
    _file << timestamp << indentation;
    switch (logType) {
    case INFO:
        // _file << "Log: ";
        break;
    case SCOPE:
        _file << "Info: ";
        break;
    case ERROR:
        _file << "Error: ";
        break;
    }
    _file << message << std::endl;
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
