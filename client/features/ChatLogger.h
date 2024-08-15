//
// Created by root on 8/11/24.
//

#ifndef MCLIB_CHATLOGGER_H
#define MCLIB_CHATLOGGER_H

#include "mclib/util/Utility.h"
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/common/internal/json.hpp>

#include <iostream>
#include "../util/Objects.h"
#include <Colors.h>

class ChatLogger : public mc::protocol::packets::PacketHandler {
public:
    MCLIB_API ChatLogger(mc::protocol::packets::PacketDispatcher* dispatcher);

    MCLIB_API void HandlePacket(mc::protocol::packets::in::ChatPacket* packet);
};


#endif //MCLIB_CHATLOGGER_H
