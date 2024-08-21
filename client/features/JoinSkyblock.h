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
#include "../util/Objects.h"
#include <Colors.h>

class JoinSkyblock : public mc::protocol::packets::PacketHandler {
public:
    MCLIB_API JoinSkyblock(mc::protocol::packets::PacketDispatcher* dispatcher);

    MCLIB_API void HandlePacket(mc::protocol::packets::in::LoginSuccessPacket* packet);
    MCLIB_API void HandlePacket(mc::protocol::packets::in::DisconnectPacket* packet);
    MCLIB_API void HandlePacket(mc::protocol::packets::in::PlayerPositionAndLookPacket* packet);

    static void SendSkyblock();
    static void SendIs();

    static bool connected;
};


#endif //MCLIB_JOINSKYBLOCK_H
