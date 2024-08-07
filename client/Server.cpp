//
// Created by root on 8/7/24.
//

#include "Logger.h"

#include <mclib/common/Common.h>
#include <mclib/core/Client.h>
#include <mclib/util/Forge.h>
#include <mclib/util/Hash.h>
#include <mclib/util/Utility.h>
#include <mclib/util/VersionFetcher.h>

#include <iostream>

#include "Server.h"

std::string Server::currentUsername = "Sandal61";
std::string Server::currentUUID = "";
std::string Server::currentSSID = "";
std::string Server::serverAddress = "stuck.hypixel.net";
u16 Server::port = 25565;

void Server::connectToServer() {
    mc::util::VersionFetcher versionFetcher(serverAddress, port);

    //std::cout << "Fetching version" << std::endl;

    auto version = mc::protocol::Version::Minecraft_1_8_9;

    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks(version);

    std::cout << "Connecting with version " << mc::protocol::to_string(version) << std::endl;

    mc::protocol::packets::PacketDispatcher dispatcher;
    mc::core::Client client(&dispatcher, version);

    versionFetcher.GetForge().SetConnection(client.GetConnection());

    client.GetPlayerController()->SetHandleFall(true);
    client.GetConnection()->GetSettings()
            .SetMainHand(mc::MainHand::Right)
            .SetViewDistance(16);

    example::Logger logger(&client, &dispatcher);

    try {
        std::cout << "Logging in." << std::endl;

        mc::core::AuthToken token;
        client.Login(serverAddress, port, currentUsername, currentUUID, currentSSID, mc::core::UpdateMethod::Block);
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
        return;
    }
}