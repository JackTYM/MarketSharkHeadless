//
// Created by root on 8/7/24.
//

#ifndef MCLIB_AUTH_H
#define MCLIB_AUTH_H
#include <string>

#include "../util/Objects.h"
#include <Colors.h>


class Auth {
public:
    static void setupWebsocket();
    static void sendToWebsocket(const std::string& type, const std::string message);
    static void sendNoLog(const std::string& type, const std::string message);
private:
};


#endif //MCLIB_AUTH_H
