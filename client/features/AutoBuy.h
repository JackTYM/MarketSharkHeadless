//
// Created by jack on 8/13/24.
//

#ifndef MCLIB_AUTOBUY_H
#define MCLIB_AUTOBUY_H

#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/core/Connection.h>
#include "QueueManager.h"
#include "../FlipItem.h"

#include "../Objects.h"

class AutoBuy : public mc::protocol::packets::PacketHandler {
public:
    MCLIB_API AutoBuy(mc::protocol::packets::PacketDispatcher* dispatcher);

    MCLIB_API void HandlePacket(mc::protocol::packets::in::SetSlotPacket* packet);

    static void autoBuy(FlipItem item);

    static int openWindow;
};



#endif //MCLIB_AUTOBUY_H
