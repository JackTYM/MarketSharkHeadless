#include <mclib/protocol/packets/PacketFactory.h>

#include <mclib/core/Connection.h>

#include <exception>
#include <string>
#include <iostream>
#include <iomanip>

namespace mc {
namespace protocol {
namespace packets {

Packet* PacketFactory::CreatePacket(Protocol& protocol, protocol::State state, DataBuffer data, std::size_t length, core::Connection* connection) {
    if (data.GetSize() == 0) return nullptr;

    VarInt vid;
    data >> vid;

    // DEBUG
    // std::cout << "Rec - " << std::hex << vid.GetInt() << std::dec << Colors::End;

    InboundPacket* packet = protocol.CreateInboundPacket(state, vid.GetInt());

    if (packet) {

        int status;
        char* demangled_name = abi::__cxa_demangle(typeid(*packet).name(), 0, 0, &status);

        // DEBUG
        /*if (status == 0) {
            std::cout << "Received Packet - " << demangled_name <<  Colors::End;
        } else {
            std::cout << "Received Packet - " << typeid(*packet).name() << Colors::End;
        }*/

        packet->SetConnection(connection);
        packet->Deserialize(data, length);
    } else {
        throw protocol::UnfinishedProtocolException(vid, state);
    }

    return packet;
}

void PacketFactory::FreePacket(Packet* packet) {
    delete packet;
}

} // ns packets
} // ns protocol
} // ns mc
