//
// Created by root on 7/31/24.
//

#ifndef MARKETSHARKCPP_TIMERDATA_H
#define MARKETSHARKCPP_TIMERDATA_H


#include <string>
#include <mclib/common/internal/json.hpp>

using json = nlohmann::json;

class TimerData {
public:
    double seconds;
    int height;
    int width;
    double scale;
    std::string prefix;
    int maxPrecision;

    TimerData() {}

    TimerData(double seconds, int height, int width, double scale, const std::string& prefix, int maxPrecision)
            : seconds(seconds), height(height), width(width), scale(scale), prefix(prefix), maxPrecision(maxPrecision) {}

    friend void to_json(json& j, const TimerData& t) {
        j = json{{"seconds", t.seconds}, {"heightPercent", t.height}, {"widthPercent", t.width}, {"scale", t.scale}, {"prefix", t.prefix}, {"maxPrecision", t.maxPrecision}};
    }

    friend void from_json(const json& j, TimerData& t) {
        j.at("seconds").get_to(t.seconds);
        j.at("heightPercent").get_to(t.height);
        j.at("widthPercent").get_to(t.width);
        j.at("scale").get_to(t.scale);
        j.at("prefix").get_to(t.prefix);
        j.at("maxPrecision").get_to(t.maxPrecision);
    }
};

#endif //MARKETSHARKCPP_TIMERDATA_H
