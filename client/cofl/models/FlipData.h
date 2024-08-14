//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_FLIPDATA_H
#define MARKETSHARKCPP_FLIPDATA_H

#include <string>
#include <vector>
#include "ChatMessageData.h"
#include "SoundData.h"
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class FlipData {
public:
    std::vector<ChatMessageData> Messages;
    std::string Id;
    int Worth;
    int Target;
    SoundData Sound;
    std::string Render;
    std::string Finder;

    FlipData() {}

    FlipData(const std::vector<ChatMessageData>& messages, const std::string& id, int worth, const SoundData& sound, const std::string& render, const std::string& finder)
            : Messages(messages), Id(id), Worth(worth), Sound(sound), Render(render), Finder(finder) {}

    friend void to_json(json& j, const FlipData& f) {
        j = json{{"messages", f.Messages}, {"id", f.Id}, {"worth", f.Worth}, {"sound", f.Sound}, {"render", f.Render}, {"finder", f.Finder}};
    }

    friend void from_json(const json& j, FlipData& f) {
        if (j.find("messages") != j.end() && !j.at("messages").is_null()) {
            j.at("messages").get_to(f.Messages);
        }

        if (j.find("id") != j.end() && !j.at("id").is_null()) {
            j.at("id").get_to(f.Id);
        } else {
            f.Id = "";
        }

        if (j.find("worth") != j.end() && !j.at("worth").is_null()) {
            j.at("worth").get_to(f.Worth);
        } else {
            f.Worth = 0;
        }

        if (j.find("target") != j.end() && !j.at("target").is_null()) {
            j.at("target").get_to(f.Target);
        } else {
            f.Target = 0;
        }

        if (j.find("sound") != j.end() && !j.at("sound").is_null()) {
            j.at("sound").get_to(f.Sound);
        }

        if (j.find("render") != j.end() && !j.at("render").is_null()) {
            j.at("render").get_to(f.Render);
        } else {
            f.Render = "";
        }

        if (j.find("finder") != j.end() && !j.at("finder").is_null()) {
            j.at("finder").get_to(f.Finder);
        } else {
            f.Finder = "";
        }
    }
};

#endif //MARKETSHARKCPP_FLIPDATA_H
