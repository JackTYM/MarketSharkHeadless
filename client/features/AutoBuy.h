//
// Created by jack on 8/13/24.
//

#ifndef MCLIB_AUTOBUY_H
#define MCLIB_AUTOBUY_H

#include "mclib/util/Utility.h"
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/core/Connection.h>
#include "QueueManager.h"
#include "../FlipItem.h"

#include "../Objects.h"

#include <thread>
#include <chrono>

class AutoBuy : public mc::protocol::packets::PacketHandler {
public:
    AutoBuy(mc::protocol::packets::PacketDispatcher* dispatcher);
    void HandlePacket(mc::protocol::packets::in::OpenWindowPacket *packet) override;
    void HandlePacket(mc::protocol::packets::in::SetSlotPacket *packet) override;
    void HandlePacket(mc::protocol::packets::in::ChatPacket *packet) override;
    static void autoBuy(FlipItem item);
private:
    static void startTimeout();
    static void endTaskIfTimedOut();
    static bool isWindowOpened;
};



#endif //MCLIB_AUTOBUY_H
