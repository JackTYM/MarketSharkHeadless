//
// Created by root on 8/11/24.
//

#include "ChatLogger.h"
#include "mclib/util/Utility.h"

ChatLogger::ChatLogger(mc::protocol::packets::PacketDispatcher *dispatcher, mc::core::Connection *connection)
        : mc::protocol::packets::PacketHandler(dispatcher), m_Connection(connection) {

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Chat, this);
}

void ChatLogger::HandlePacket(mc::protocol::packets::in::ChatPacket *packet) {
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
        if (message.length() > 0)
            std::cout << std::string(message.begin(), message.end()) << "\n";
    }
}