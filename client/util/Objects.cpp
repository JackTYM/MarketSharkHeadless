//
// Created by jack on 8/13/24.
//

#include "Objects.h"

mc::core::Connection* Objects::m_Connection;
ix::WebSocket Objects::coflWebSocket;
ix::WebSocket Objects::msWebSocket;
bool Objects::debug = true;

std::string Objects::currentUsername = "Sandal61";
std::string Objects::currentUUID;
std::string Objects::currentSSID;
std::string Objects::serverAddress = "192.168.7.201";
u16 Objects::port = 25566;

int Objects::openWindowId = 0;
int Objects::actionNumber = 1;

bool Objects::skip = true;
int Objects::skipDelay = 15;