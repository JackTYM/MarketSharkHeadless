//
// Created by root on 8/11/24.
//

#ifndef MCLIB_JOINSKYBLOCK_H
#define MCLIB_JOINSKYBLOCK_H

#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/protocol/packets/PacketDispatcher.h>


#include <mclib/core/Connection.h>

#include <iostream>
#include <thread>
#include <chrono>

#include "../cofl/CoflNet.h"

class JoinSkyblock : public mc::protocol::packets::PacketHandler {
private:
    mc::core::Connection* m_Connection;
    bool connected = false;
public:
    MCLIB_API JoinSkyblock(mc::protocol::packets::PacketDispatcher* dispatcher, mc::core::Connection* connection);

    MCLIB_API void HandlePacket(mc::protocol::packets::in::LoginSuccessPacket* packet);
    MCLIB_API void HandlePacket(mc::protocol::packets::in::DisconnectPacket* packet);
    MCLIB_API void HandlePacket(mc::protocol::packets::in::PlayerPositionAndLookPacket* packet);

    void SendSkyblock();
    void SendIs();
};


#endif //MCLIB_JOINSKYBLOCK_H
