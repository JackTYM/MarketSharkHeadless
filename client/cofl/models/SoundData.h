//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_SOUNDDATA_H
#define MARKETSHARKCPP_SOUNDDATA_H


#include <string>
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class SoundData {
public:
    std::string Name;
    float Pitch;

    SoundData() {}

    SoundData(const std::string& name, float pitch)
            : Name(name), Pitch(pitch) {}

    friend void to_json(json& j, const SoundData& s) {
        j = json{{"name", s.Name}, {"pitch", s.Pitch}};
    }

    friend void from_json(const json& j, SoundData& s) {
        if (j.find("name") != j.end() && !j.at("name").is_null()) {
            j.at("name").get_to(s.Name);
        } else {
            s.Name = "";
        }

        if (j.find("pitch") != j.end() && !j.at("pitch").is_null()) {
            j.at("pitch").get_to(s.Pitch);
        } else {
            s.Pitch = 0;
        }
    }
};

#endif //MARKETSHARKCPP_SOUNDDATA_H
