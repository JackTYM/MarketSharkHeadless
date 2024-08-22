//
// Created by jack on 8/21/24.
//

#include "ColorConfig.h"
// Define static variables
std::string ColorConfig::CoflMessage = Colors::WhiteBackground;
std::string ColorConfig::Important = Colors::YellowBackground;
std::string ColorConfig::ChatMessage = Colors::Gray;
std::string ColorConfig::Won = Colors::Yellow;
std::string ColorConfig::BuySpeed = Colors::Green;
std::string ColorConfig::Coins = Colors::Magenta;
std::string ColorConfig::ServerStatus = Colors::Green;
std::string ColorConfig::Error = Colors::Red;
std::string ColorConfig::Disconnection = Colors::Red;
std::string ColorConfig::FlipInfo = Colors::Cyan;
std::string ColorConfig::Debug = Colors::Black;

const std::string& configFilePath = "colors.json";

void ColorConfig::loadColors() {
    std::ifstream configFile(configFilePath);
    nlohmann::json jsonConfig;

    if (configFile.is_open()) {
        configFile >> jsonConfig;
    }

    CoflMessage = getColorFromJson(jsonConfig, "CoflMessage", CoflMessage);
    Important = getColorFromJson(jsonConfig, "Important", Important);
    ChatMessage = getColorFromJson(jsonConfig, "ChatMessage", ChatMessage);
    Won = getColorFromJson(jsonConfig, "Won", Won);
    BuySpeed = getColorFromJson(jsonConfig, "BuySpeed", BuySpeed);
    Coins = getColorFromJson(jsonConfig, "Coins", Coins);
    ServerStatus = getColorFromJson(jsonConfig, "ServerStatus", ServerStatus);
    Error = getColorFromJson(jsonConfig, "Error", Error);
    Disconnection = getColorFromJson(jsonConfig, "Disconnection", Disconnection);
    FlipInfo = getColorFromJson(jsonConfig, "FlipInfo", FlipInfo);
    Debug = getColorFromJson(jsonConfig, "Debug", Debug);
}

void ColorConfig::saveColors() {
    nlohmann::json jsonConfig;

    setColorToJson(jsonConfig, "CoflMessage", CoflMessage);
    setColorToJson(jsonConfig, "Important", Important);
    setColorToJson(jsonConfig, "ChatMessage", ChatMessage);
    setColorToJson(jsonConfig, "Won", Won);
    setColorToJson(jsonConfig, "BuySpeed", BuySpeed);
    setColorToJson(jsonConfig, "Coins", Coins);
    setColorToJson(jsonConfig, "ServerStatus", ServerStatus);
    setColorToJson(jsonConfig, "Error", Error);
    setColorToJson(jsonConfig, "Disconnection", Disconnection);
    setColorToJson(jsonConfig, "FlipInfo", FlipInfo);
    setColorToJson(jsonConfig, "Debug", Debug);

    std::ofstream configFile(configFilePath);
    if (configFile.is_open()) {
        configFile << jsonConfig.dump(4); // Pretty print with 4 spaces
    }
}

std::string ColorConfig::getColorFromJson(const nlohmann::json& jsonConfig, const std::string& key, const std::string& defaultColor) {
    if (jsonConfig.find(key) != jsonConfig.end()) {
        std::string colorName = jsonConfig[key];
        if (colorName == "Black") return Colors::Black;
        else if (colorName == "Red") return Colors::Red;
        else if (colorName == "Green") return Colors::Green;
        else if (colorName == "Yellow") return Colors::Yellow;
        else if (colorName == "Blue") return Colors::Blue;
        else if (colorName == "Magenta") return Colors::Magenta;
        else if (colorName == "Cyan") return Colors::Cyan;
        else if (colorName == "White") return Colors::White;
        else if (colorName == "Gray") return Colors::Gray;
        else if (colorName == "Grey") return Colors::Gray;
        else if (colorName == "BlackBackground") return Colors::BlackBackground;
        else if (colorName == "RedBackground") return Colors::RedBackground;
        else if (colorName == "GreenBackground") return Colors::GreenBackground;
        else if (colorName == "YellowBackground") return Colors::YellowBackground;
        else if (colorName == "BlueBackground") return Colors::BlueBackground;
        else if (colorName == "MagentaBackground") return Colors::MagentaBackground;
        else if (colorName == "CyanBackground") return Colors::CyanBackground;
        else if (colorName == "WhiteBackground") return Colors::WhiteBackground;
        else if (colorName == "GrayBackground") return Colors::GrayBackground;
        else if (colorName == "GreyBackground") return Colors::GrayBackground;
        else if (colorName == "BrightBlack") return Colors::BrightBlack;
        else if (colorName == "BrightRed") return Colors::BrightRed;
        else if (colorName == "BrightGreen") return Colors::BrightGreen;
        else if (colorName == "BrightYellow") return Colors::BrightYellow;
        else if (colorName == "BrightBlue") return Colors::BrightBlue;
        else if (colorName == "BrightMagenta") return Colors::BrightMagenta;
        else if (colorName == "BrightCyan") return Colors::BrightCyan;
        else if (colorName == "BrightWhite") return Colors::BrightWhite;
        else if (colorName == "BrightBlackBackground") return Colors::BrightBlackBackground;
        else if (colorName == "BrightRedBackground") return Colors::BrightRedBackground;
        else if (colorName == "BrightGreenBackground") return Colors::BrightGreenBackground;
        else if (colorName == "BrightYellowBackground") return Colors::BrightYellowBackground;
        else if (colorName == "BrightBlueBackground") return Colors::BrightBlueBackground;
        else if (colorName == "BrightMagentaBackground") return Colors::BrightMagentaBackground;
        else if (colorName == "BrightCyanBackground") return Colors::BrightCyanBackground;
        else if (colorName == "BrightWhiteBackground") return Colors::BrightWhiteBackground;
        else if (colorName == "None") return Colors::None;
    }
    return defaultColor;
}

void ColorConfig::setColorToJson(nlohmann::json& jsonConfig, const std::string& key, const std::string& color) {
    if (color == Colors::Black) jsonConfig[key] = "Black";
    else if (color == Colors::Red) jsonConfig[key] = "Red";
    else if (color == Colors::Green) jsonConfig[key] = "Green";
    else if (color == Colors::Yellow) jsonConfig[key] = "Yellow";
    else if (color == Colors::Blue) jsonConfig[key] = "Blue";
    else if (color == Colors::Magenta) jsonConfig[key] = "Magenta";
    else if (color == Colors::Cyan) jsonConfig[key] = "Cyan";
    else if (color == Colors::White) jsonConfig[key] = "White";
    else if (color == Colors::Gray) jsonConfig[key] = "Gray";
    else if (color == Colors::BlackBackground) jsonConfig[key] = "BlackBackground";
    else if (color == Colors::RedBackground) jsonConfig[key] = "RedBackground";
    else if (color == Colors::GreenBackground) jsonConfig[key] = "GreenBackground";
    else if (color == Colors::YellowBackground) jsonConfig[key] = "YellowBackground";
    else if (color == Colors::BlueBackground) jsonConfig[key] = "BlueBackground";
    else if (color == Colors::MagentaBackground) jsonConfig[key] = "MagentaBackground";
    else if (color == Colors::CyanBackground) jsonConfig[key] = "CyanBackground";
    else if (color == Colors::WhiteBackground) jsonConfig[key] = "WhiteBackground";
    else if (color == Colors::GrayBackground) jsonConfig[key] = "GrayBackground";
    else if (color == Colors::BrightBlack) jsonConfig[key] = "BrightBlack";
    else if (color == Colors::BrightRed) jsonConfig[key] = "BrightRed";
    else if (color == Colors::BrightGreen) jsonConfig[key] = "BrightGreen";
    else if (color == Colors::BrightYellow) jsonConfig[key] = "BrightYellow";
    else if (color == Colors::BrightBlue) jsonConfig[key] = "BrightBlue";
    else if (color == Colors::BrightMagenta) jsonConfig[key] = "BrightMagenta";
    else if (color == Colors::BrightCyan) jsonConfig[key] = "BrightCyan";
    else if (color == Colors::BrightWhite) jsonConfig[key] = "BrightWhite";
    else if (color == Colors::BrightBlackBackground) jsonConfig[key] = "BrightBlackBackground";
    else if (color == Colors::BrightRedBackground) jsonConfig[key] = "BrightRedBackground";
    else if (color == Colors::BrightGreenBackground) jsonConfig[key] = "BrightGreenBackground";
    else if (color == Colors::BrightYellowBackground) jsonConfig[key] = "BrightYellowBackground";
    else if (color == Colors::BrightBlueBackground) jsonConfig[key] = "BrightBlueBackground";
    else if (color == Colors::BrightMagentaBackground) jsonConfig[key] = "BrightMagentaBackground";
    else if (color == Colors::BrightCyanBackground) jsonConfig[key] = "BrightCyanBackground";
    else if (color == Colors::BrightWhiteBackground) jsonConfig[key] = "BrightWhiteBackground";
    else if (color == Colors::None) jsonConfig[key] = "None";
}