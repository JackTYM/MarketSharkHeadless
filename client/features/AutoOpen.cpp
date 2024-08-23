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
            std::cout << ColorConfig::FlipInfo << "Opening " << item.auctionId << " Item Name - " << item.strippedDisplayName << " Target " << item.coflWorth << Colors::End;
        });
    }
}