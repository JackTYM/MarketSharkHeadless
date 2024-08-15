//
// Created by jack on 8/13/24.
//

#include "Failsafes.h"

Failsafes::Failsafes(mc::protocol::packets::PacketDispatcher *dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher) {

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Chat, this);
}

void Failsafes::HandlePacket(mc::protocol::packets::in::ChatPacket *packet) {
    if (packet->GetChatPosition() == mc::protocol::packets::in::ChatPacket::ChatPosition::ChatBox) {
        const nlohmann::json &root = packet->GetChatData();

        std::string message = mc::util::ParseChatNode(root);
        std::size_t pos = message.find((char) 0xA7);

        while (pos != std::string::npos) {
            message.erase(pos - 1, 3);
            pos = message.find((char) 0xA7);
        }

        message.erase(std::remove_if(message.begin(), message.end(), [](char c) {
            return c < 32 || c > 126;
        }), message.end());
        if (message.length() > 0) {
            if (message.contains("You cannot view this auction!")) {
                std::cout << "No cookie! Closing cofl." << Colors::End;
                Objects::coflWebSocket.stop();
            }
        }
    }
}