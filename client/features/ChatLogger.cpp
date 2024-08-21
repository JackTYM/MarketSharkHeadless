//
// Created by root on 8/11/24.
//

#include <codecvt>
#include "ChatLogger.h"

ChatLogger::ChatLogger(mc::protocol::packets::PacketDispatcher *dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher) {

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Chat, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Disconnect, this);
    dispatcher->RegisterHandler(mc::protocol::State::Login, mc::protocol::login::Disconnect, this);
}

void ChatLogger::HandlePacket(mc::protocol::packets::in::DisconnectPacket *packet) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::cout << Colors::Red << "Disconnected! " << converter.to_bytes(packet->GetReason()) << Colors::End;
}

void ChatLogger::HandlePacket(mc::protocol::packets::in::ChatPacket *packet) {
    if (packet->GetChatPosition() == mc::protocol::packets::in::ChatPacket::ChatPosition::ChatBox) {
        const nlohmann::json &root = packet->GetChatData();

        std::string message = mc::util::ParseChatNode(root);

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

            message = Colors::BrightYellowBackground + message;
        } else {
            message = Colors::stripColorCodes(message);

            message = Colors::Gray + message;
        }

        if (!message.empty()) {
            std::cout << message << Colors::End;
        }
    }
}
