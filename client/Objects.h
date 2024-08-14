//
// Created by jack on 8/13/24.
//

#ifndef MCLIB_OBJECTS_H
#define MCLIB_OBJECTS_H

#include <mclib/core/Connection.h>
#include <ixwebsocket/IXWebSocket.h>

class Objects {
public:
    static mc::core::Connection* m_Connection;
    static ix::WebSocket coflWebSocket;
    static ix::WebSocket msWebSocket;
    static bool debug;

    static std::string currentUsername;
    static std::string currentUUID;
    static std::string currentSSID;

    static std::string serverAddress;
    static uint16_t port;
};


#endif //MCLIB_OBJECTS_H
