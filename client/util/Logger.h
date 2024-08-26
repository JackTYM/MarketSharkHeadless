//
// Created by jack on 8/26/24.
//

#ifndef MCLIB_LOGGER_H
#define MCLIB_LOGGER_H


#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Logger {
public:
    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    static void initialize() {
        log_file.open("log.txt", std::ios::out | std::ios::app);
        if (!log_file.is_open()) {
            std::cerr << "Failed to open log file: " << "log.txt" << std::endl;
        }

        log_file << "\n\n" << std::endl;
    }

    static void log(const std::string &message) {
        std::string timestamped_message = addTimestamp(message);
        std::cout << timestamped_message << std::endl;
        if (log_file.is_open()) {
            log_file << timestamped_message << std::endl;
        }
    }

private:
    static std::ofstream log_file;

    static std::string addTimestamp(const std::string &message) {
        std::time_t now = std::time(nullptr);
        char buf[100];
        std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S] ", std::localtime(&now));
        return std::string(buf) + message;
    }
};


#endif //MCLIB_LOGGER_H
