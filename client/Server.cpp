//
// Created by root on 8/7/24.
//

#include <mclib/common/Common.h>
#include <mclib/core/Client.h>
#include <mclib/util/Forge.h>
#include <mclib/util/Hash.h>
#include <mclib/util/Utility.h>
#include <mclib/util/VersionFetcher.h>

#include <iostream>
#include <string>
#include <thread>

#include "Server.h"
#include "Logger.h"

#include "features/JoinSkyblock.h"
#include "features/ChatLogger.h"

std::string Server::currentUsername = "Sandal61";
std::string Server::currentUUID = "";
std::string Server::currentSSID = "";
std::string Server::serverAddress = "mc.hypixel.net";
u16 Server::port = 25565;

void Server::connectToServer() {
    mc::util::VersionFetcher versionFetcher(serverAddress, port);

    auto version = versionFetcher.GetVersion();

    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks(version);

    std::cout << "Connecting with version " << mc::protocol::to_string(version) << std::endl;

    mc::core::Client client(&versionFetcher.GetDispatcher(), version);

    versionFetcher.GetForge().SetConnection(client.GetConnection());

    client.GetPlayerController()->SetHandleFall(true);
    client.GetConnection()->GetSettings()
            .SetMainHand(mc::MainHand::Right)
            .SetViewDistance(16);

    JoinSkyblock joinSkyblock(&versionFetcher.GetDispatcher(), client.GetConnection());
    ChatLogger chatLogger(&versionFetcher.GetDispatcher(), client.GetConnection());

    example::Logger logger(&client, &versionFetcher.GetDispatcher());

    try {
        std::cout << "Logging in." << std::endl;

        mc::core::AuthToken token;
        client.Login(serverAddress, port, currentUsername, currentUUID, currentSSID, mc::core::UpdateMethod::Block);
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
        return;
    }
}