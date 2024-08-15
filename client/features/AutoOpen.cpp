//
// Created by root on 8/11/24.
//

#include "AutoOpen.h"

void AutoOpen::OpenAuction(FlipItem item) {
    if (QueueManager::getCurrentActionName().empty()) {
        QueueManager::addTaskToStart("AutoBuy", [&item]() {
            Objects::m_Connection->SendPacket(mc::protocol::packets::out::CloseWindowPacket(Objects::openWindowId));
            AutoBuy::autoBuy(item);
            Objects::m_Connection->SendPacket(mc::protocol::packets::out::ChatPacket("/viewauction " + item.auctionId));
            std::cout << "Running " << ("/viewauction " + item.auctionId) << std::endl;
        });
    }
}