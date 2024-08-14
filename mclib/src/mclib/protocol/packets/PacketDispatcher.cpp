#include <mclib/protocol/packets/PacketDispatcher.h>

#include <mclib/protocol/packets/PacketHandler.h>

#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <iomanip>

namespace mc {
namespace protocol {
namespace packets {

s32 GetDispatcherId(Packet* packet) {
    auto version = packet->GetProtocolVersion();

    protocol::Protocol protocol = protocol::Protocol::GetProtocol(version);

    s32 agnosticId = 0;
    if (!protocol.GetAgnosticId(packet->GetProtocolState(), packet->GetId().GetInt(), agnosticId)) {
        throw std::runtime_error(std::string("Unknown packet type ") + std::to_string(packet->GetId().GetInt()) + " received");
    }

    return agnosticId;
}

void PacketDispatcher::RegisterHandler(protocol::State protocolState, PacketId id, PacketHandler* handler) {
    PacketType type(protocolState, id);
    std::vector<PacketHandler*>::iterator found = std::find(m_Handlers[type].begin(), m_Handlers[type].end(), handler);
    if (found == m_Handlers[type].end()) {
        m_Handlers[type].push_back(handler);

        //std::cout << "Registering Handler " << static_cast<int>(protocolState) << "-0x" << id << " Len: " << m_Handlers.size() << std::endl;
    }
}

void PacketDispatcher::UnregisterHandler(protocol::State protocolState, PacketId id, PacketHandler* handler) {
    //std::cout << "Unregistering Handler!" << std::endl;
    PacketType type(protocolState, id);
    std::vector<PacketHandler*>::iterator found = std::find(m_Handlers[type].begin(), m_Handlers[type].end(), handler);
    if (found != m_Handlers[type].end())
        m_Handlers[type].erase(found);
}

void PacketDispatcher::UnregisterHandler(PacketHandler* handler) {
    //std::cout << "UnRegistering Handler!" << std::endl;
    for (auto& pair : m_Handlers) {
        if (pair.second.empty()) continue;

        auto state = pair.first.first;
        auto id = pair.first.second;
        PacketType type(state, id);

        m_Handlers[type].erase(std::remove(m_Handlers[type].begin(), m_Handlers[type].end(), handler), m_Handlers[type].end());
    }
}

void PacketDispatcher::Dispatch(Packet* packet) {
    if (!packet) return;

    auto state = packet->GetProtocolState();
    s64 id = GetDispatcherId(packet);

    PacketType type(state, id);
    //std::cout << "Dispatching " << static_cast<int>(state) << "-0x" << id << " - " << m_Handlers[type].size() << std::endl;

    for (PacketHandler* handler : m_Handlers[type]) {
        packet->Dispatch(handler);
        //std::cout << "Dispatched to handler" << std::endl;
    }
}

} // ns packets
} // ns protocol
} // ns mc
