#include <iostream>
#include <string>

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
    std::cout << "  autoupdate   Toggles auto-update [on/off]\n";
}

void handleCommand(const std::string& cmd) {
    if (cmd == "help") {
        showHelp();
    } else if (cmd == "status") {
        std::cout << "Showing status...\n";
    } else if (cmd == "login") {
        std::cout << "Logging in...\n";
    } else if (cmd == "logout") {
        std::cout << "Logging out...\n";
    } else if (cmd == "accounts") {
        std::cout << "Listing accounts...\n";
    } else if (cmd == "addaccount") {
        std::cout << "Adding new account...\n";
    } else if (cmd == "delaccount") {
        std::cout << "Deleting account...\n";
    } else if (cmd == "start" || cmd == "restart") {
        std::cout << "Starting MarketShark...\n";
    } else if (cmd == "stop") {
        std::cout << "Stopping MarketShark...\n";
    } else if (cmd == "logs") {
        std::cout << "Opening logs...\n";
    } else {
        std::cout << "Unknown command. Type 'ms help' for a list of available commands.\n";
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        showHelp();
        return 0;
    }

    std::string command = argv[1];
    handleCommand(command);
    return 0;
}