//
// Created by root on 8/11/24.
//

#include "JoinSkyblock.h"
#include <ColorConfig.h>

bool JoinSkyblock::connected = false;

JoinSkyblock::JoinSkyblock(mc::protocol::packets::PacketDispatcher *dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher)
{

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::PlayerPositionAndLook, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Disconnect, this);
    dispatcher->RegisterHandler(mc::protocol::State::Login, mc::protocol::login::LoginSuccess, this);
}

void JoinSkyblock::SendIs() {
    // Wait 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (connected) {
        Logger::log(ColorConfig::ServerStatus + "Joining Island!" + Colors::End);

        mc::protocol::packets::out::ChatPacket packet("/is");
        Objects::m_Connection->SendPacket(&packet);

        CoflNet::setupWebsocket();
    }
}

void JoinSkyblock::SendSkyblock() {
    // Wait 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    if (connected) {
        Logger::log(ColorConfig::ServerStatus + "Joining Skyblock!" + Colors::End);

        mc::protocol::packets::out::ChatPacket packet("/skyblock");
        Objects::m_Connection->SendPacket(&packet);

        SendIs();
    }
}

void JoinSkyblock::HandlePacket(mc::protocol::packets::in::LoginSuccessPacket* packet) {
    connected = true;

    Logger::log(ColorConfig::ServerStatus + "Joined Server!" + Colors::End);

    std::thread(&JoinSkyblock::SendSkyblock).detach();
}

void JoinSkyblock::HandlePacket(mc::protocol::packets::in::PlayerPositionAndLookPacket* packet) {
    connected = true;

    //Logger::log("Teleported!" + Colors::End);
}

void JoinSkyblock::HandlePacket(mc::protocol::packets::in::DisconnectPacket* packet) {
    connected = false;
}