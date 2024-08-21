// CommandType.h

#ifndef MARKETSHARKCPP_COMMANDTYPE_H
#define MARKETSHARKCPP_COMMANDTYPE_H

#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

enum class CommandType {
    WriteToChat,
    set,
    Execute,
    TokenLogin,
    Clicked,
    PlaySound,
    ChatMessage,
    PurchaseStart,
    PurchaseConfirm,
    Reset,
    Flip,
    PrivacySettings,
    Countdown,
    updatePurse,
    updateBits,
    updateServer,
    updateLocation,
    chatBatch,
    uploadTab,
    uploadScoreboard,
    GetMods,
    ProxyRequest,
    GetScoreboard,
    Ignoring,
    GetInventory
};

class CommandTypeHelper {
public:
    static std::map<CommandType, std::string> data;

    static void initialize() {
        data[CommandType::WriteToChat] = "writeToChat";
        data[CommandType::set] = "set";
        data[CommandType::Execute] = "execute";
        data[CommandType::TokenLogin] = "tokenLogin";
        data[CommandType::Clicked] = "clicked";
        data[CommandType::PlaySound] = "playSound";
        data[CommandType::ChatMessage] = "chatMessage";
        data[CommandType::PurchaseStart] = "purchaseStart";
        data[CommandType::PurchaseConfirm] = "purchaseConfirm";
        data[CommandType::Reset] = "reset";
        data[CommandType::Flip] = "flip";
        data[CommandType::PrivacySettings] = "privacySettings";
        data[CommandType::Countdown] = "countdown";
        data[CommandType::updatePurse] = "updatePurse";
        data[CommandType::updateBits] = "updateBits";
        data[CommandType::updateServer] = "updateServer";
        data[CommandType::updateLocation] = "updateLocation";
        data[CommandType::chatBatch] = "chatBatch";
        data[CommandType::uploadTab] = "uploadTab";
        data[CommandType::uploadScoreboard] = "uploadScoreboard";
        data[CommandType::GetMods] = "getMods";
        data[CommandType::ProxyRequest] = "proxy";
        data[CommandType::GetScoreboard] = "getScoreboard";
        data[CommandType::GetInventory] = "getInventory";
    }

    static std::string toJson(CommandType type) {
        if (data.find(type) != data.end()) {
            return data[type];
        }
        return "UnknownType";
    }

    static CommandType fromJson(const std::string& str) {
        for (const auto& pair : data) {
            if (pair.second == str) {
                return pair.first;
            }
        }
        return CommandType::Ignoring;
    }
};

inline std::ostream& operator<<(std::ostream& os, CommandType type) {
    os << CommandTypeHelper::toJson(type);
    return os;
}

#endif //MARKETSHARKCPP_COMMANDTYPE_H
