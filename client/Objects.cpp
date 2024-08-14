//
// Created by jack on 8/13/24.
//

#include "Objects.h"

mc::core::Connection* Objects::m_Connection;
ix::WebSocket Objects::coflWebSocket;
ix::WebSocket Objects::msWebSocket;
bool Objects::debug = false;

std::string Objects::currentUsername = "Sandal61";
std::string Objects::currentUUID = "";
std::string Objects::currentSSID = "";
std::string Objects::serverAddress = "mc.hypixel.net";
u16 Objects::port = 25565;