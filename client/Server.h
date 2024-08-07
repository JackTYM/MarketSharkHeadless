//
// Created by root on 8/7/24.
//

#ifndef MCLIB_SERVER_H
#define MCLIB_SERVER_H


class Server {
public:
    static void connectToServer();

    static std::string currentUsername;
    static std::string currentUUID;
    static std::string currentSSID;

    static std::string serverAddress;
    static uint16_t port;
};


#endif //MCLIB_SERVER_H
