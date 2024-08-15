#include "Logger.h"

#include "mclib/util/Utility.h"

#include <iostream>
#include <codecvt>

namespace example {

Logger::Logger(mc::core::Client* client, mc::protocol::packets::PacketDispatcher* dispatcher)
    : mc::protocol::packets::PacketHandler(dispatcher), m_Client(client)
{
    using namespace mc::protocol;

    m_Client->RegisterListener(this);

    dispatcher->RegisterHandler(State::Login, login::Disconnect, this);

    dispatcher->RegisterHandler(State::Play, play::Chat, this);
    dispatcher->RegisterHandler(State::Play, play::Disconnect, this);
    dispatcher->RegisterHandler(State::Play, play::EntityLookAndRelativeMove, this);
    dispatcher->RegisterHandler(State::Play, play::BlockChange, this);
    dispatcher->RegisterHandler(State::Play, play::MultiBlockChange, this);
}

Logger::~Logger() {
    GetDispatcher()->UnregisterHandler(this);
    m_Client->UnregisterListener(this);
}

void Logger::HandlePacket(mc::protocol::packets::in::ChatPacket* packet) {
    std::string message = mc::util::ParseChatNode(packet->GetChatData());


}

void Logger::HandlePacket(mc::protocol::packets::in::EntityLookAndRelativeMovePacket* packet) {
    mc::Vector3d delta = mc::ToVector3d(packet->GetDelta()) / (128.0 * 32.0);

    //std::cout << delta << Colors::End;
}

void Logger::HandlePacket(mc::protocol::packets::in::BlockChangePacket* packet) {
    mc::Vector3i pos = packet->GetPosition();
    s32 blockId = packet->GetBlockId();

    //std::cout << "Block changed at " << pos << " to " << blockId << Colors::End;
}

void Logger::HandlePacket(mc::protocol::packets::in::MultiBlockChangePacket* packet) {
    auto chunkX = packet->GetChunkX();
    auto chunkZ = packet->GetChunkZ();

    for (const auto& change : packet->GetBlockChanges()) {
        mc::Vector3i pos(chunkX + change.x, change.y + chunkZ + change.z);

        //std::cout << "Block changed at " << pos << " to " << change.blockData << Colors::End;
    }
}

void Logger::HandlePacket(mc::protocol::packets::in::DisconnectPacket* packet) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::cout << "Disconnected: " << converter.to_bytes(packet->GetReason()) << Colors::End;
}

void Logger::OnTick() {
    mc::core::PlayerPtr player = m_Client->GetPlayerManager()->GetPlayerByName(L"testplayer");
    if (!player) return;

    mc::entity::EntityPtr entity = player->GetEntity();
    if (!entity) return;
}

} // ns example
