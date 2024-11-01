#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "env.h"

void showHelp() {
    std::cout << "marketshark (ms) - Command Line Interface for MarketShark\n\n";
    std::cout << "Usage:\n";
    std::cout << "  ms <command>\n\n";
    std::cout << "Available commands:\n";
    std::cout << "  status       Prints the account and running status of the program\n";
    std::cout << "  login        Logs into MarketShark\n";
    std::cout << "  logout       Logs out of MarketShark\n";
    std::cout << "  accounts     Lists your active Minecraft sessions\n";
    std::cout << "  addaccount   Adds a new Minecraft account\n";
    std::cout << "  delaccount   Removes a Minecraft account\n";
    std::cout << "  start        Starts or restarts the MarketShark program\n";
    std::cout << "  stop         Stops the MarketShark program\n";
    std::cout << "  logs         Brings up the logs for MarketShark\n";
    std::cout << "  update       Downloads and updates the latest MarketShark program\n";
    std::cout << "  fullupdate   Downloads and updates the MarketShark cli and service\n";
    std::cout << "  autoupdate   Toggles auto-update [on/off]\n";
    std::cout << "  timer        Runs the program for a specific amount of hours, then stops it\n";
    std::cout << "  config       Opens up the config file using nano\n";
    std::cout << "  colors       Opens up the colors file using nano\n";
}

void requestService(const std::string &command) {
    struct sockaddr_un address{};
    int sock;
    char buffer[1024] = {0};

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error \n";
        return;
    }

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "/tmp/marketshark.sock");

    if (connect(sock, (struct sockaddr *) &address, sizeof(address)) < 0) {
        std::cerr << "Could not connect to MarketShark service. Please run 'ms fullupdate'\n";
        close(sock);
        return;
    }

    send(sock, command.c_str(), command.length(), 0);
    int bytesRead;
    while ((bytesRead = read(sock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout << buffer << std::flush;

        if (std::string(buffer).contains("Started MarketShark!") ||
            std::string(buffer).contains("Restarted MarketShark!")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            system(("tail -f " + std::string(std::getenv("HOME")) + "/.marketshark/color_log.txt").c_str());
            break;
        }
    }

    if (bytesRead < 0) {
        std::cerr << "Error reading from socket\n";
    }

    close(sock);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        showHelp();
        return 0;
    }

    std::string cmd = argv[1];
    std::string fullCommand;

    if (cmd == "help") {
        showHelp();
    } else if (cmd == "status") {
        fullCommand = "status";
    } else if (cmd == "login") {
        if (argc < 3) {
            std::cout << "Usage: ms login (key)\n";
            return 0;
        }
        fullCommand = "login " + std::string(argv[2]);
    } else if (cmd == "logout") {
        fullCommand = "logout";
    } else if (cmd == "accounts") {
        fullCommand = "accounts";
    } else if (cmd == "addaccount") {
        fullCommand = "addaccount";
    } else if (cmd == "delaccount") {
        if (argc < 3) {
            std::cout << "Usage: ms delaccount (username)\n";
            return 0;
        }
        fullCommand = "delaccount " + std::string(argv[2]);
    } else if (cmd == "start" || cmd == "restart") {
        fullCommand = "start";
    } else if (cmd == "stop") {
        fullCommand = "stop";
    } else if (cmd == "logs") {
        system(("tail -f " + std::string(std::getenv("HOME")) + "/.marketshark/color_log.txt").c_str());
        return 0;
    } else if (cmd == "update") {
        fullCommand = "update";
    } else if (cmd == "fullupdate") {
        system((std::string("curl -sSL \"https://service.") + DOMAIN + std::string("/install.sh\" -o /tmp/install.sh && sudo bash /tmp/install.sh &")).c_str());
    } else if (cmd == "autoupdate") {
        if (argc < 3) {
            std::cout << "Usage: ms autoupdate (on / off)\n";
            return 0;
        }
        fullCommand = "autoupdate " + std::string(argv[2]);
    } else if (cmd == "timer") {
        if (argc < 3) {
            std::cout << "Usage: ms timer (hours)\n";
            return 0;
        }
        fullCommand = "timer " + std::string(argv[2]);
    } else if (cmd == "config") {
        system(("nano " + std::string(std::getenv("HOME")) + "/.marketshark/config.json").c_str());
    } else if (cmd == "color" || cmd == "colors") {
        system(("nano " + std::string(std::getenv("HOME")) + "/.marketshark/colors.json").c_str());
    } else {
        std::cout << "Unknown command. Type 'ms help' for a list of available commands.\n";
        return 0;
    }

    // If fullCommand is set, send it to requestService
    if (!fullCommand.empty()) {
        requestService(fullCommand);
    }

    return 0;
}