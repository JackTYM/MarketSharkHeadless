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
            std::unordered_map<char, std::string> colorMap = {
                    {'0', Colors::Black},
                    {'1', Colors::Blue},
                    {'2', Colors::Green},
                    {'3', Colors::Cyan},
                    {'4', Colors::Red},
                    {'5', Colors::Magenta},
                    {'6', Colors::Yellow},
                    {'7', Colors::White},
                    {'8', Colors::Gray},
                    {'9', Colors::BrightBlue},
                    {'a', Colors::BrightGreen},
                    {'b', Colors::BrightCyan},
                    {'c', Colors::BrightRed},
                    {'d', Colors::BrightMagenta},
                    {'e', Colors::BrightYellow},
                    {'f', Colors::BrightWhite},
                    {'l', Colors::Bold},
                    {'n', Colors::Underline},
                    {'r', Colors::ResetHighlight}
            };

            std::size_t pos = message.find((char) 0xA7);
            while (pos != std::string::npos && pos + 1 < message.length()) {
                char code = message[pos + 1];
                auto it = colorMap.find(code);
                if (it != colorMap.end()) {
                    message.replace(pos, 2, it->second);
                } else {
                    message.erase(pos, 2);
                }
                pos = message.find((char) 0xA7, pos);
            }

            message = Colors::BrightYellowBackground + message;
        } else {
            // Remove color codes if not preserving them
            std::size_t pos = message.find((char) 0xA7);
            while (pos != std::string::npos) {
                message.erase(pos, 2);
                pos = message.find((char) 0xA7);
            }

            // Remove any non-printable characters
            message.erase(std::remove_if(message.begin(), message.end(), [](char c) {
                return c < 32 || c > 126;
            }), message.end());

            message = Colors::Gray + message;
        }

        if (!message.empty()) {
            std::cout << message << Colors::End;
        }
    }
}
