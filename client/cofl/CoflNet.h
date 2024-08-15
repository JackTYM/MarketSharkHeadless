//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_COFLNET_H
#define MARKETSHARKCPP_COFLNET_H

#include <string>
#include <mclib/common/internal/json.hpp>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <iostream>
#include "JsonStringCommand.h"
#include "models/ChatMessageData.h"
#include "CoflSessionManager.h"
#include "models/FlipData.h"
#include "RawCommand.h"

#include <mclib/core/Connection.h>

#include "../features/AutoOpen.h"
#include "../Server.h"
#include "../util/FlipItem.h"
#include "../util/Objects.h"
#include <Colors.h>

class CoflNet {
public:
    static void setupWebsocket();

    static void sendToWebsocket(const std::string &type, const std::string message);

private:
};


#endif //MARKETSHARKCPP_COFLNET_H
