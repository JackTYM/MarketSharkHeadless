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

long running = 0;
pid_t msPid = 0;
bool loggedIn = false;

ix::WebSocket websocket;
std::string currentKey;
int server_fd, client_socket;

void send(const std::string &type, const std::string message) {
    // Using nlohmann::json for JSON construction
    nlohmann::json jsonObject;
    jsonObject["type"] = type;
    jsonObject["message"] = message;

    jsonObject["key"] = currentKey;
    jsonObject["version"] = "SERVICE";

    std::string jsonString = jsonObject.dump();

    websocket.send(jsonString);
}

void loginWebsocket() {
    websocket.setUrl("wss://service.marketshark.net");

    websocket.setOnMessageCallback([](const ix::WebSocketMessagePtr &msg) {
        switch (msg->type) {
            case ix::WebSocketMessageType::Open:
                send("Login", "");
                break;
            case ix::WebSocketMessageType::Close:
                std::cout << "Closed! " << msg->str << std::endl;
            case ix::WebSocketMessageType::Error:
                std::cout << "Error! " << std::endl;
                std::cout << "Error code: " << msg->errorInfo.reason << std::endl;
                std::cout << "HTTP Status: " << msg->errorInfo.http_status << std::endl;
                loggedIn = false;
                break;
            case ix::WebSocketMessageType::Message:
                nlohmann::json parsed_json = nlohmann::json::parse(msg->str);

                std::string type = parsed_json["type"];

                std::string message = parsed_json["message"];
                std::cout << msg->str << std::endl;

                if (type == "LoggedIn") {
                    loggedIn = true;
                    std::cout << "Successfully authenticated as " + parsed_json["message"].get<std::string>()
                              << std::endl;
                } else if (type == "ValidKey") {
                    std::string response = "Key is valid! Saved.\n";

                    std::ofstream outputFile("key");
                    if (outputFile.is_open()) {
                        currentKey = parsed_json["key"].get<std::string>();
                        outputFile << currentKey;
                        outputFile.close();
                        loggedIn = true;

                        send("Login", "");
                    } else {
                        response = "Failed to log in. Are files setup correctly?";
                    }

                    write(client_socket, response.c_str(), response.length());
                    close(client_socket);
                } else if (type == "InvalidKey") {
                    std::string response = "Key is invalid! Please copy and paste directly from the MarketShark bot!";
                    write(client_socket, response.c_str(), response.length());
                    close(client_socket);
                } else if (type == "AccountList") {
                    std::vector<std::string> accounts = nlohmann::json::parse(message).get<std::vector<std::string>>();
                    std::string response =
                            "You have " + std::to_string(accounts.size()) + " account(s) connected to MarketShark!";

                    for (std::string account: accounts) {
                        response += "\n - " + account;
                    }

                    response += "\n";

                    write(client_socket, response.c_str(), response.length());
                    close(client_socket);
                } else if (type == "AuthenticateAccount") {
                    std::string response = "Use the following link to log into Minecraft\n" + message;
                    write(client_socket, response.c_str(), response.length());
                } else if (type == "AccountLoggedIn") {
                    std::string response = "\n\nSuccessfully logged into Minecraft with account " + message + "\n";
                    write(client_socket, response.c_str(), response.length());
                    close(client_socket);
                } else if (type == "AccountNotFound") {
                    std::string response =
                            "Account " + message + " was not found. Please make sure the username is in ms accounts.\n";
                    write(client_socket, response.c_str(), response.length());
                    close(client_socket);
                } else if (type == "DeletedAccount") {
                    std::string response = "Deleted " + message + " from accounts.\n";
                    write(client_socket, response.c_str(), response.length());
                    close(client_socket);
                }
                break;
        }
    });

    websocket.start();
}

void startExecutable(bool sendResponse) {
    std::string response;

    if (loggedIn) {
        std::string path = (std::filesystem::current_path().string() + "/executable");
        std::filesystem::path executable{path};

        if (std::filesystem::exists(executable)) {
            bool success = true;
            if (msPid != 0) {
                if (kill(msPid, SIGKILL) == -1) {
                    response = "Failed to stop MarketShark executable. Report this! Reason: ";
                    response += strerror(errno);
                    success = false;
                } else {
                    waitpid(msPid, NULL, 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

                    std::thread([]() {
                        int status;
                        pid_t result = waitpid(msPid, &status, 0);
                        if (result == msPid) {
                            if (WIFEXITED(status)) {
                                int exitStatus = WEXITSTATUS(status);
                                std::cout << "MarketShark exited with status " + std::to_string(exitStatus) << ". Restarting." << std::endl;

                                startExecutable(false);
                            } else if (WIFSIGNALED(status)) {
                                int signalNumber = WTERMSIG(status);
                                std::cout << "Child process terminated by signal: " +
                                             std::to_string(signalNumber) << ". Can ignore?" << std::endl;
                            }
                        }
                    }).detach();
                } else {
                    execlp(path.c_str(), path.c_str(), (char *) NULL);

                    perror("Failed to start executable");
                    _exit(1);
                }
            }
        } else {
            response = "Executable not downloaded. Please run ms update";
        }
    } else {
        response = "You are not logged in. Use ms login.";
    }

    if (sendResponse) {
        write(client_socket, response.c_str(), response.length());
        close(client_socket);
    }
}

int main() {
    std::ifstream key("key");
    std::getline(key, currentKey);

    loginWebsocket();

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
        client_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addr_len);
        char buffer[1024] = {0};
        read(client_socket, buffer, 1024);
        std::cout << "Command received: " << buffer << std::endl;

        std::string response;

        std::string command(buffer);

        if (command == "status") {
            if (loggedIn) {
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

                    std::ifstream configFile("config.json");
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
            send("TestKey", command.substr(6));

            continue;
        } else if (command == "logout") {
            if (loggedIn) {
                currentKey = "";
                loggedIn = false;
                remove("key");

                response = "Successfully logged out.";
            } else {
                response = "You are not logged in!";
            }
        } else if (command == "accounts") {
            if (loggedIn) {
                send("GetAccounts", "");

                continue;
            } else {
                response = "You are not logged in. Use ms login.";
            }
        } else if (command == "addaccount") {
            if (loggedIn) {
                send("AddAccount", "");

                continue;
            } else {
                response = "You are not logged in. Use ms login.";
            }
        } else if (command.starts_with("delaccount")) {
            if (loggedIn) {
                send("DelAccount", command.substr(11));

                continue;
            } else {
                response = "You are not logged in. Use ms login.";
            }
        } else if (command == "start") {
            startExecutable(true);

            continue;
        } else if (command == "stop") {
            if (loggedIn) {
                if (msPid == 0) {
                    response = "MarketShark not running!";
                } else {
                    if (kill(msPid, SIGKILL) == -1) {
                        response = "Failed to stop MarketShark executable. Report this! Reason: ";
                        response += strerror(errno);
                    } else {
                        waitpid(msPid, NULL, 0);
                        response = "Stopped MarketShark!";
                        msPid = 0;
                        running = 0;
                    }
                }
            } else {
                response = "You are not logged in. Use ms login.";
            }
        } else if (command == "update") {
            if (loggedIn) {
                bool restart = false;
                bool success = true;
                if (msPid != 0) {
                    restart = true;
                    if (kill(msPid, SIGKILL) == -1) {
                        response = "Failed to stop MarketShark executable. Report this! Reason: ";
                        response += strerror(errno);
                        success = false;
                    } else {
                        waitpid(msPid, NULL, 0);
                        msPid = 0;
                        running = 0;
                    }
                }

                if (success) {
                    std::string rmCmd = "rm -f ~/.marketshark/executable";
                    int result = system(rmCmd.c_str());

                    if (result != 0) {
                        response = "Failed to delete executable.";
                    } else {
                        std::string curlCmd =
                                "curl -o ~/.marketshark/executable \"https://service.marketshark.net/executable?key=" +
                                currentKey + "\"";
                        int result2 = system(curlCmd.c_str());

                        if (result2 != 0) {
                            response = "Failed to download executable.";
                        } else {
                            std::string chmodCmd = "chmod +x ~/.marketshark/executable";
                            int result3 = system(chmodCmd.c_str());

                            if (result3 != 0) {
                                response = "Failed to chmod executable.";
                            } else {
                                response = "Successfully downloaded MarketShark!";
                                if (restart) {

                                }
                            }
                        }
                    }
                }
            } else {
                response = "You are not logged in. Use ms login.";
            }
        } else if (command.starts_with("autoupdate")) {
            response = "This aint done yet chief, gimmie a bit";

            /*if (loggedIn) {
                response = "Autoupdate command executed with option: " + command.substr(11);
            } else {
                response = "You are not logged in. Use ms login.";
            }*/
        } else if (command.starts_with("timer")) {
            response = "This aint done yet chief, gimmie a bit";

            /*if (loggedIn) {
                response = "Timer command executed for hours: " + command.substr(6);
            } else {
                response = "You are not logged in. Use ms login.";
            }*/
        } else {
            response = "Unknown command received";
        }

        response += "\n";

        write(client_socket, response.c_str(), response.length());
        close(client_socket);
    }
}