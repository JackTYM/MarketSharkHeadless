//
// Created by jack on 8/13/24.
//

#include "Failsafes.h"
#include "JoinSkyblock.h"

Failsafes::Failsafes(mc::protocol::packets::PacketDispatcher *dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher) {

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Chat, this);
}

void Failsafes::HandlePacket(mc::protocol::packets::in::ChatPacket *packet) {
    if (packet->GetChatPosition() == mc::protocol::packets::in::ChatPacket::ChatPosition::ChatBox) {
        const nlohmann::json &root = packet->GetChatData();

        std::string message = mc::util::ParseChatNode(root);
        message = Colors::stripColorCodes(message);

        if (!message.empty()) {
            if (message.contains("You cannot view this auction!")) {
                std::cout << "No cookie! Closing cofl." << Colors::End;
                Objects::coflWebSocket.stop();
            } else if (message.contains("A disconnect occurred in your connection, so you were put in the SkyBlock Lobby!")) {
                JoinSkyblock::connected = false;
                std::thread([]() {
                    JoinSkyblock::SendSkyblock();
                }).detach();
            }
        }
    }
}