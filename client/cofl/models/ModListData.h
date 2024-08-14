//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_MODLISTDATA_H
#define MARKETSHARKCPP_MODLISTDATA_H


#include <vector>
#include <string>
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class ModListData {
private:
    std::vector<std::string> fileNames;
    std::vector<std::string> modNames;
    std::vector<std::string> fileHashes;

public:
    void addFilename(const std::string& name) {
        fileNames.push_back(name);
    }

    void addModname(const std::string& modname) {
        modNames.push_back(modname);
    }

    void addFileHashes(const std::string& hash) {
        fileHashes.push_back(hash);
    }

    friend void to_json(json& j, const ModListData& m) {
        j = json{{"fileNames", m.fileNames}, {"modNames", m.modNames}, {"fileHashes", m.fileHashes}};
    }

    friend void from_json(const json& j, ModListData& m) {
        j.at("fileNames").get_to(m.fileNames);
        j.at("modNames").get_to(m.modNames);
        j.at("fileHashes").get_to(m.fileHashes);
    }
};

#endif //MARKETSHARKCPP_MODLISTDATA_H
