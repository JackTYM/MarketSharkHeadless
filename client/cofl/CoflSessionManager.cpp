#include "CoflSessionManager.h"

std::string GenerateUUID() {
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_str[37]; // UUID is 36 characters plus null terminator
    uuid_unparse(uuid, uuid_str);
    return std::string(uuid_str);
}

std::filesystem::path GetUserPath(const std::string& username) {
    // Implement the logic to get the path based on username
    return std::filesystem::path(username);
}

void OverwriteCoflSession(const std::string& username, const CoflSession& session) {
    std::filesystem::path path = GetUserPath(username);
    std::ofstream file(path);
    if (!file) {
        throw std::ios_base::failure("Failed to open file for writing");
    }
    nlohmann::json j = session;
    file << j.dump(4); // Pretty print with 4 spaces
}

CoflSession GetCoflSession(const std::string& username) {
    std::filesystem::path path = GetUserPath(username);
    if (!std::filesystem::exists(path)) {
        CoflSession session{GenerateUUID(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()};
        OverwriteCoflSession(username, session);
        return session;
    }

    std::ifstream file(path);
    if (!file) {
        throw std::ios_base::failure("Failed to open file for reading");
    }

    std::string raw((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

    nlohmann::json j = nlohmann::json::parse(raw);
    CoflSession session = j.get<CoflSession>();

    return session;
}