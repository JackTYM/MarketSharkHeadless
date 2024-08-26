//
// Created by jack on 8/25/24.
//

#ifndef MCLIB_STATS_H
#define MCLIB_STATS_H

#include "mclib/util/Utility.h"
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/common/internal/json.hpp>

#include <iostream>
#include "../util/Objects.h"
#include <Colors.h>

class Stats : public mc::protocol::packets::PacketHandler {
public:
    MCLIB_API Stats(mc::protocol::packets::PacketDispatcher* dispatcher);

    MCLIB_API void HandlePacket(mc::protocol::packets::in::TeamsPacket* packet);
    MCLIB_API void HandlePacket(mc::protocol::packets::in::StatisticsPacket* packet);

    static std::string scoreboard[13];

    static std::string purse;
    static std::string island;
    static std::string visitors;
    static std::string hypixelPing;
    static std::string coflPing;
    static std::string coflDelay;

    static long pingTime;
};


#endif //MCLIB_STATS_H
