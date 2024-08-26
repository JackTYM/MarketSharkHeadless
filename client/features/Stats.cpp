//
// Created by jack on 8/25/24.
//

#include <codecvt>
#include "Stats.h"

std::string Stats::scoreboard[13];

std::string Stats::purse;
std::string Stats::island;
std::string Stats::visitors;
std::string Stats::hypixelPing;
std::string Stats::coflPing;
std::string Stats::coflDelay;

long Stats::pingTime;

Stats::Stats(mc::protocol::packets::PacketDispatcher *dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher) {

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Teams, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Statistics, this);
}

void Stats::HandlePacket(mc::protocol::packets::in::TeamsPacket *packet) {
    std::wstring_convert <std::codecvt_utf8<wchar_t>> converter;

    if (packet->GetMode() == mc::protocol::packets::in::TeamsPacket::Mode::Update || packet->GetMode() == mc::protocol::packets::in::TeamsPacket::Mode::Create) {
        if (packet->GetTeamName().starts_with(L"team_")) {
            scoreboard[std::stoi(packet->GetTeamName().substr(5))-1] = converter.to_bytes(packet->GetTeamPrefix() + packet->GetTeamSuffix());

            if (std::stoi(packet->GetTeamName().substr(5))-1 == 6) {
                Stats::purse = Colors::stripColorCodes(Stats::scoreboard[6]).substr(7);
            } else if (std::stoi(packet->GetTeamName().substr(5))-1 == 8) {
                Stats::island = Colors::stripColorCodes(Stats::scoreboard[8]);
            }
            //std::cout << std::stoi(packet->GetTeamName().substr(5))-1 << ":" << converter.to_bytes(packet->GetTeamPrefix() + packet->GetTeamSuffix()) << std::endl;
        }
    }
}

void Stats::HandlePacket(mc::protocol::packets::in::StatisticsPacket *packet) {
    hypixelPing = std::to_string(((duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - pingTime) / 2)) + "ms";
}