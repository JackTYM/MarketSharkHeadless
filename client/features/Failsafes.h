//
// Created by jack on 8/13/24.
//

#ifndef MCLIB_FAILSAFES_H
#define MCLIB_FAILSAFES_H

#include "mclib/util/Utility.h"
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/common/internal/json.hpp>

#include <iostream>
#include "../Objects.h"

#include "QueueManager.h"

class Failsafes : public mc::protocol::packets::PacketHandler {
public:
    MCLIB_API Failsafes(mc::protocol::packets::PacketDispatcher* dispatcher);

    MCLIB_API void HandlePacket(mc::protocol::packets::in::ChatPacket* packet);
};


#endif //MCLIB_FAILSAFES_H
