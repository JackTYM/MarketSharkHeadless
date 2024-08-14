//
// Created by root on 8/1/24.
//

#ifndef MARKETSHARKCPP_COFLSESSIONMANAGER_H
#define MARKETSHARKCPP_COFLSESSIONMANAGER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <uuid/uuid.h>
#include <mclib/common/internal/json.hpp>

struct CoflSession {
    std::string id;
    long time;

    friend void to_json(nlohmann::json& j, const CoflSession& session) {
        j = nlohmann::json{{"id", session.id}, {"time", session.time}};
    }

    // Function to convert JSON to struct
    friend void from_json(const nlohmann::json& j, CoflSession& session) {
        j.at("id").get_to(session.id);
        j.at("time").get_to(session.time);
    }
};

// Function declarations
std::string GenerateUUID();

std::filesystem::path GetUserPath(const std::string& username);

void OverwriteCoflSession(const std::string& username, const CoflSession& session);

CoflSession GetCoflSession(const std::string& username);

#endif //MARKETSHARKCPP_COFLSESSIONMANAGER_H
