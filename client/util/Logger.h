#ifndef MCLIB_LOGGER_H
#define MCLIB_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Logger {
public:
    ~Logger();
    static void initialize();
    static void log(const std::string &message);

private:
    static std::ofstream log_file;
    static std::string addTimestamp(const std::string &message);
};

#endif //MCLIB_LOGGER_H
