//
// Created by root on 8/11/24.
//

#include "AutoOpen.h"

mc::core::Connection* AutoOpen::m_Connection;

AutoOpen::AutoOpen(mc::core::Connection *connection) {
    m_Connection = connection;
}

void AutoOpen::OpenAuction(std::string message) {
    QueueManager::addToStartOfQueue([]() {

    });
    //mc::protocol::packets::out::ChatPacket packet(message);
    //m_Connection->SendPacket(&packet);
}