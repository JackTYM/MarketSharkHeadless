#include "Logger.h"
#include "Colors.h"
#include <regex>

std::ofstream Logger::log_file;
std::ofstream Logger::color_log_file;

Logger::~Logger() {
    if (log_file.is_open()) {
        log_file.close();
    }
    if (color_log_file.is_open()) {
        color_log_file << "Program Ended!" << std::flush;
        color_log_file.close();
    }
}

void Logger::initialize() {
    log_file.open("log.txt", std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file: " << "log.txt" << std::endl;
    }

    color_log_file.open("color_log.txt", std::ios::out);
    if (!color_log_file.is_open()) {
        std::cerr << "Failed to open color log file: " << "color_log.txt" << std::endl;
    }

    log_file << "\n\nNew Session " + addTimestamp("") + "\n";
}

void Logger::log(const std::string &message) {
    std::string timestamped_message = addTimestamp(message);
    if (!log_file.is_open() || !color_log_file.is_open()) {
        initialize();
    }
    std::regex colorCodePattern("\033\\[[0-9;]*m");
    log_file << std::regex_replace(timestamped_message, colorCodePattern, "") << std::flush;

    std::cout << message;
    color_log_file << message << std::flush;
}

std::string Logger::addTimestamp(const std::string &message) {
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S] ", std::localtime(&now));
    return std::string(buf) + message;
}