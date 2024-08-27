#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "../mclib/include/mclib/common/internal/json.hpp"

long running = 0;
std::string homeDir = std::getenv("HOME");

int main() {
    int server_fd, client_socket;
    struct sockaddr_un address;
    int addr_len = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "/tmp/marketshark.sock");
    unlink("/tmp/marketshark.sock");

    bind(server_fd, (struct sockaddr *) &address, sizeof(address));
    listen(server_fd, 3);

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addr_len);
        read(client_socket, buffer, 1024);
        std::cout << "Command received: " << buffer << std::endl;

        std::string response;

        std::string command(buffer);

        if (command == "status") {
            std::ifstream inputFile(homeDir + "/.marketshark/key");
            std::string line;

            if (inputFile.is_open()) {
                while (getline(inputFile, line)) {
                    std::cout << line << std::endl;
                }
                inputFile.close();

                std::cout << line << std::endl;

                response = "MarketShark Status: ";
                if (running != 0) {
                    response += "Running for ";

                    long time = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch()).count() - running;

                    std::ostringstream stream;
                    int totalMinutes = time / 60000;
                    int hours = totalMinutes / 60;
                    int minutes = totalMinutes % 60;

                    stream << hours << "H " << minutes << "m on account ";
                    response += stream.str();

                    std::ifstream configFile(homeDir + "/.marketshark/config.json");
                    nlohmann::json jsonConfig;
                    if (configFile.is_open()) {
                        configFile >> jsonConfig;
                    }

                    if (jsonConfig["currentUsername"] != nullptr) {
                        response += jsonConfig["currentUsername"].get<std::string>() + ".";
                    } else {
                        response += "Unknown account.";
                    }
                } else {
                    response += "Stopped.";
                }
            } else {
                response = "Not logged in. Please run ms login";
            }
        } else if (command.rfind("login", 0) == 0) {
            std::cout << homeDir + "/.marketshark/key" << std::endl;
            std::ofstream outputFile(homeDir + "/.marketshark/key");
            if (outputFile.is_open()) {
                outputFile << command.substr(6);
                outputFile.close();

                response = "Saved key!";
            } else {
                response = "Failed to log in. Are files setup correctly?";
            }
        } else if (command == "logout") {
            response = "Logout command executed";
        } else if (command == "accounts") {
            response = "Accounts command executed";
        } else if (command == "addaccount") {
            response = "Add account command executed";
        } else if (command.rfind("delaccount", 0) == 0) { // Checks if command starts with "delaccount"
            response = "Delete account command executed for user: " + command.substr(11);
        } else if (command == "start") {
            if (running == 0) {
                response = "Started MarketShark!";
            } else {
                response = "Restarted MarketShark!";
            }
            running = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
        } else if (command == "stop") {
            response = "Stop command executed";
        } else if (command == "logs") {
            response = "Logs command executed";
        } else if (command == "update") {
            response = "Update command executed";
        } else if (command.rfind("autoupdate", 0) == 0) { // Checks if command starts with "autoupdate"
            response = "Autoupdate command executed with option: " + command.substr(11);
        } else if (command.rfind("timer", 0) == 0) { // Checks if command starts with "timer"
            response = "Timer command executed for hours: " + command.substr(6);
        } else {
            response = "Unknown command received";
        }

        write(client_socket, response.c_str(), response.length());
        close(client_socket);
    }
}