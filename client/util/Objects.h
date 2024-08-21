//
// Created by jack on 8/13/24.
//

#ifndef MCLIB_OBJECTS_H
#define MCLIB_OBJECTS_H

#include "mclib/core/Connection.h"
#include <ixwebsocket/IXWebSocket.h>
#include <string>
#include <fstream>

class Objects {
public:
    // Variables not stored in config
    static mc::core::Connection* m_Connection;
    static ix::WebSocket coflWebSocket;
    static ix::WebSocket msWebSocket;
    static std::string currentUUID;
    static std::string currentSSID;
    static std::string serverAddress;
    static u16 port;
    static int openWindowId;
    static int actionNumber;
    static std::string msSession;

    static void loadConfig();
    static void saveConfig();

    // Getters
    static bool getDebug();
    static std::string getKey();
    static std::string getCurrentUsername();
    static int getBedSpamStartDelay();
    static int getBedSpamDelay();
    static bool getUsSocket();
    static bool getBafSocket();

    // Setters
    static void setDebug(bool value);
    static void setKey(const std::string& key);
    static void setCurrentUsername(const std::string& username);
    static void setBedSpamStartDelay(int delay);
    static void setBedSpamDelay(int delay);
    static void setUsSocket(bool value);
    static void setBafSocket(bool value);

    // Functions
    static void sendToWebsocket(const std::string& type, const std::string message);
    static void sendNoLog(const std::string& type, const std::string message);

    static void sendRawCommand(const std::string& type, const std::string& data);

    static std::string replaceAll(std::string str, const std::string& from, const std::string& to);
    static std::string removeChar(std::string str, char32_t from);
private:
    static nlohmann::json loadJson();
    static void saveJson(const nlohmann::json& json);

    static std::string configFilePath;

    static bool debug;
    static std::string key;
    static std::string currentUsername;
    static int bedSpamStartDelay;
    static int bedSpamDelay;
    static bool usSocket;
    static bool bafSocket;
};


#endif //MCLIB_OBJECTS_H
