//
// Created by root on 8/11/24.
//

#include "JoinSkyblock.h"

JoinSkyblock::JoinSkyblock(mc::protocol::packets::PacketDispatcher *dispatcher, mc::core::Connection *connection)
        : mc::protocol::packets::PacketHandler(dispatcher), m_Connection(connection)
{

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::PlayerPositionAndLook, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Disconnect, this);
    dispatcher->RegisterHandler(mc::protocol::State::Login, mc::protocol::login::LoginSuccess, this);
}

void JoinSkyblock::SendIs() {
    // Wait 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (connected) {
        std::cout << "Joining Island!" << std::endl;

        mc::protocol::packets::out::ChatPacket packet("/is");
        m_Connection->SendPacket(&packet);

        CoflNet::setupWebsocket();
    }
}

void JoinSkyblock::SendSkyblock() {
    // Wait 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    if (connected) {
        std::cout << "Joining Skyblock!" << std::endl;

        mc::protocol::packets::out::ChatPacket packet("/skyblock");
        m_Connection->SendPacket(&packet);

        SendIs();
    }
}

void JoinSkyblock::HandlePacket(mc::protocol::packets::in::LoginSuccessPacket* packet) {
    connected = true;

    std::cout << "Joined Server!" << std::endl;

    std::thread delayThread(&JoinSkyblock::SendSkyblock, this);

    delayThread.detach();
}

void JoinSkyblock::HandlePacket(mc::protocol::packets::in::PlayerPositionAndLookPacket* packet) {
    connected = true;

    //std::cout << "Teleported!" << std::endl;
}

void JoinSkyblock::HandlePacket(mc::protocol::packets::in::DisconnectPacket* packet) {
    connected = false;
}