//
// Created by jack on 8/13/24.
//

#include "Objects.h"
#include "../cofl/RawCommand.h"

// Temporary Variables
mc::core::Connection* Objects::m_Connection;
ix::WebSocket Objects::coflWebSocket;
ix::WebSocket Objects::msWebSocket;

std::string Objects::currentUUID;
std::string Objects::currentSSID;
std::string Objects::serverAddress = "mc.hypixel.net";
u16 Objects::port = 25565;

int Objects::openWindowId = 0;
int Objects::actionNumber = 1;

std::string Objects::msSession = "";

std::string Objects::configFilePath = "config.json";

nlohmann::json Objects::loadJson() {
    std::ifstream configFile(configFilePath);
    nlohmann::json jsonConfig;
    if (configFile.is_open()) {
        configFile >> jsonConfig;
    }
    return jsonConfig;
}

void Objects::saveJson(const nlohmann::json& json) {
    std::ofstream configFile(configFilePath);
    if (configFile.is_open()) {
        configFile << json.dump(4); // Pretty print with 4 spaces
    }
}

// Default Values
bool Objects::debug = false;
std::string Objects::key = "";
std::string Objects::currentUsername = "";
int Objects::bedSpamStartDelay = 19900;
int Objects::bedSpamDelay = 50;
bool Objects::usSocket = true;
bool Objects::bafSocket = true;

void Objects::loadConfig() {
    nlohmann::json jsonConfig = loadJson();
    if (!jsonConfig.empty()) {
        Objects::debug = jsonConfig.value("debug", debug);
        Objects::key = jsonConfig.value("key", key);
        Objects::currentUsername = jsonConfig.value("currentUsername", currentUsername);
        Objects::bedSpamStartDelay = jsonConfig.value("bedSpamStartDelay", bedSpamStartDelay);
        Objects::bedSpamDelay = jsonConfig.value("bedSpamDelay", bedSpamDelay);
        Objects::usSocket = jsonConfig.value("usSocket", usSocket);
        Objects::bafSocket = jsonConfig.value("bafSocket", bafSocket);
    } else {
        saveConfig();
        loadConfig();
    }
}

void Objects::saveConfig() {
    nlohmann::json jsonConfig;
    jsonConfig["debug"] = Objects::debug;
    jsonConfig["key"] = Objects::key;
    jsonConfig["currentUsername"] = Objects::currentUsername;
    jsonConfig["bedSpamStartDelay"] = Objects::bedSpamStartDelay;
    jsonConfig["bedSpamDelay"] = Objects::bedSpamDelay;
    jsonConfig["usSocket"] = Objects::usSocket;
    jsonConfig["bafSocket"] = Objects::bafSocket;
    saveJson(jsonConfig);
}

// Getters
bool Objects::getDebug() {
    loadConfig();
    return debug;
}

std::string Objects::getKey() {
    loadConfig();
    return key;
}

std::string Objects::getCurrentUsername() {
    loadConfig();
    return currentUsername;
}

int Objects::getBedSpamStartDelay() {
    loadConfig();
    return bedSpamStartDelay;
}

int Objects::getBedSpamDelay() {
    loadConfig();
    return bedSpamDelay;
}

bool Objects::getUsSocket() {
    loadConfig();
    return usSocket;
}

bool Objects::getBafSocket() {
    loadConfig();
    return bafSocket;
}

// Setters
void Objects::setDebug(bool value) {
    debug = value;
    saveConfig();
}

void Objects::setCurrentUsername(const std::string& username) {
    currentUsername = username;
    saveConfig();
}

void Objects::setKey(const std::string& newKey) {
    key = newKey;
    saveConfig();
}

void Objects::setBedSpamStartDelay(int delay) {
    bedSpamStartDelay = delay;
    saveConfig();
}

void Objects::setBedSpamDelay(int delay) {
    bedSpamDelay = delay;
    saveConfig();
}

void Objects::setUsSocket(bool value) {
    usSocket = value;
    saveConfig();
}

void Objects::setBafSocket(bool value) {
    bafSocket = value;
    saveConfig();
}

void Objects::sendToWebsocket(const std::string &type, const std::string message) {
    // Using nlohmann::json for JSON construction
    nlohmann::json jsonObject;
    jsonObject["type"] = type;
    jsonObject["message"] = message;

    std::string jsonString = jsonObject.dump();

    if (Objects::getDebug()) {
        std::cout << Colors::Black << "Sending: " << jsonString << Colors::End;
    }

    sendNoLog(type, message);
}


void Objects::sendNoLog(const std::string &type, const std::string message) {
    // Using nlohmann::json for JSON construction
    nlohmann::json jsonObject;
    jsonObject["type"] = type;
    jsonObject["message"] = message;

    jsonObject["key"] = Objects::getKey();
    jsonObject["username"] = Objects::getCurrentUsername();
    jsonObject["hwid"] = "";
    jsonObject["version"] = "GREATWHITE";
    jsonObject["modVersion"] = "1.0.0";

    if (!Objects::msSession.empty()) {
        jsonObject["session_id"] = Objects::msSession;
    }

    std::string jsonString = jsonObject.dump();

    Objects::msWebSocket.send(jsonString);
}

void Objects::sendRawCommand(const std::string& type, const std::string& data) {
    RawCommand rc(type, data);
    nlohmann::json cmd;

    cmd["type"] = rc.getType();
    cmd["data"] = rc.getData();

    Objects::coflWebSocket.send(cmd.dump());
}

std::string Objects::replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t startPos = 0;
    while((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length();
    }
    return str;
}

std::string Objects::removeChar(std::string str, char32_t from) {
    size_t startPos = 0;
    while((startPos = str.find(from, startPos)) != std::string::npos) {
        str.erase(startPos, 1); // Remove the character
    }
    return str;
}