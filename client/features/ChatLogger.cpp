//
// Created by root on 8/11/24.
//

#include <codecvt>
#include "ChatLogger.h"
#include <ColorConfig.h>

std::list<std::string> ChatLogger::chatList;

ChatLogger::ChatLogger(mc::protocol::packets::PacketDispatcher *dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher) {

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Chat, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Disconnect, this);
    dispatcher->RegisterHandler(mc::protocol::State::Login, mc::protocol::login::Disconnect, this);
}

void ChatLogger::HandlePacket(mc::protocol::packets::in::DisconnectPacket *packet) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    Logger::log(ColorConfig::Disconnection + "Disconnected! " + converter.to_bytes(packet->GetReason()) + Colors::End);
}

void ChatLogger::HandlePacket(mc::protocol::packets::in::ChatPacket *packet) {
    if (packet->GetChatPosition() == mc::protocol::packets::in::ChatPacket::ChatPosition::ChatBox) {
        const nlohmann::json &root = packet->GetChatData();

        std::string message = mc::util::ParseChatNode(root);

        chatList.push_back(Colors::stripColorCodes(message));

        // Array of strings that should preserve color codes
        std::vector<std::string> preserveStrings = {"You purchased ", "Putting coins in escrow",
                                                    "Visit the Auction House ", "Sending to server "};

        bool preserveColorCodes = false;
        for (const auto &str : preserveStrings) {
            if (message.find(str) != std::string::npos) {
                preserveColorCodes = true;
                break;
            }
        }

        if (message.contains("pingwarspls")) {
            Objects::m_Connection->SendPacket(mc::protocol::packets::out::ChatPacket("/social pingwars"));
        }

        if (preserveColorCodes) {
            message = Colors::convertColorCodes(message);

            message = ColorConfig::Important + message;
        } else {
            message = Colors::stripColorCodes(message);

            message = ColorConfig::ChatMessage + message;
        }

        if (!message.empty()) {
            Logger::log(message + Colors::End);
        }
    }
}
