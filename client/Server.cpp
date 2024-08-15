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
#include "util/Logger.h"

#include "features/JoinSkyblock.h"
#include "features/ChatLogger.h"
#include "features/AutoBuy.h"
#include "features/Failsafes.h"

void Server::connectToServer() {
    mc::util::VersionFetcher versionFetcher(Objects::serverAddress, Objects::port);

    auto version = versionFetcher.GetVersion();

    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks(version);

    std::cout << Colors::Green << "Connecting with version " << mc::protocol::to_string(version) << Colors::End;

    mc::core::Client client(&versionFetcher.GetDispatcher(), version);

    versionFetcher.GetForge().SetConnection(client.GetConnection());

    client.GetPlayerController()->SetHandleFall(true);
    client.GetConnection()->GetSettings()
            .SetMainHand(mc::MainHand::Right)
            .SetViewDistance(16);

    Objects::m_Connection = client.GetConnection();

    JoinSkyblock joinSkyblock(&versionFetcher.GetDispatcher());
    ChatLogger chatLogger(&versionFetcher.GetDispatcher());
    AutoBuy autoBuy(&versionFetcher.GetDispatcher());
    Failsafes failsafes(&versionFetcher.GetDispatcher());

    example::Logger logger(&client, &versionFetcher.GetDispatcher());

    try {
        std::cout << Colors::Green << "Logging in." << Colors::End;

        mc::core::AuthToken token;
        client.Login(Objects::serverAddress, Objects::port, Objects::currentUsername, Objects::currentUUID, Objects::currentSSID, mc::core::UpdateMethod::Block);
    } catch (std::exception& e) {
        std::cout << e.what() << Colors::End;
        return;
    }
}