#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <thread>
#include <cstdlib>
#include <sys/wait.h>
#include "../mclib/include/mclib/common/internal/json.hpp"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include "mclib/common/Json.h"

long running = 0;
pid_t msPid = 0;
std::string homeDir = std::getenv("HOME");
bool loggedIn = false;

void loginWebsocket() {

}

int main() {


    // Ensure marketshark directory exists
    mkdir((homeDir + "/.marketshark/").c_str(), 0755);

    int server_fd, client_socket;
    struct sockaddr_un address;
    int addr_len = sizeof(address);

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
        char buffer[1024] = {0};
        read(client_socket, buffer, 1024);
        std::cout << "Command received: " << buffer << std::endl;

        std::string response;

        std::string command(buffer);

        if (command == "status") {
            std::ifstream inputFile(homeDir + "/.marketshark/key");
            std::string line;

            if (inputFile.is_open()) {
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
        } else if (command.starts_with("login")) {
            std::ofstream outputFile(homeDir + "/.marketshark/key");
            if (outputFile.is_open()) {
                outputFile << command.substr(6);
                outputFile.close();

                response = "Saved key!";
            } else {
                response = "Failed to log in. Are files setup correctly?";
            }
        } else if (command == "logout") {
            remove((homeDir + "/.marketshark/key").c_str());

            response = "Successfully logged out.";
        } else if (command == "accounts") {
            response = "Not implemented yet sorry";
        } else if (command == "addaccount") {
            response = "Use discord for now, sorry";
        } else if (command.starts_with("delaccount")) {
            response = "Not implemented yet, sorry";
        } else if (command == "start") {
            std::string path = (homeDir + "/.marketshark/marketshark");
            std::filesystem::path executable{path};

            if (std::filesystem::exists(executable)) {
                bool success = true;
                if (msPid != 0) {
                    if (kill(msPid, SIGKILL) == -1) {
                        response = "Failed to stop MarketShark executable. Report this!";
                        success = false;
                    } else {
                        waitpid(msPid, NULL, 0);
                        running = 0;
                    }
                }
                if (success) {
                    pid_t pid = fork();

                    if (pid == -1) {
                        response = "Failed to start executable. Report this! (Failed to fork: pid = -1)";
                    } else if (pid > 0) {
                        if (msPid == 0) {
                            response = "Started MarketShark!";
                        } else {
                            response = "Restarted MarketShark!";
                        }
                        msPid = pid;
                        running = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::system_clock::now().time_since_epoch()).count();
                    } else {
                        execlp(path.c_str(), path.c_str(), (char *) NULL);
                    }
                }
            } else {
                response = "Executable not downloaded. Please run ms update";
            }
        } else if (command == "stop") {
            if (msPid == 0) {
                response = "MarketShark not running!";
            } else {
                if (kill(msPid, SIGKILL) == -1) {
                    response = "Failed to stop MarketShark executable. Report this!";
                } else {
                    waitpid(msPid, NULL, 0);
                    response = "Stopped MarketShark!";
                    msPid = 0;
                    running = 0;
                }
            }
        } else if (command == "update") {

        } else if (command.starts_with("autoupdate")) {
            response = "Autoupdate command executed with option: " + command.substr(11);
        } else if (command.starts_with("timer")) {
            response = "Timer command executed for hours: " + command.substr(6);
        } else {
            response = "Unknown command received";
        }

        write(client_socket, response.c_str(), response.length());
        close(client_socket);
    }
}