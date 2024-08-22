//
// Created by jack on 8/21/24.
//

#ifndef MCLIB_COLORCONFIG_H
#define MCLIB_COLORCONFIG_H


#include <string>
#include "mclib/core/Connection.h"
#include <fstream>
#include <Colors.h>

class ColorConfig {
public:
    // Public variables for each type
    static std::string CoflMessage;
    static std::string Important;
    static std::string ChatMessage;
    static std::string Won;
    static std::string BuySpeed;
    static std::string Coins;
    static std::string ServerStatus;
    static std::string Error;
    static std::string Disconnection;
    static std::string FlipInfo;
    static std::string Debug;

    // Load colors from JSON
    static void loadColors();

    static void saveColors();

private:
    // Helper function to get color from JSON
    static std::string getColorFromJson(const nlohmann::json& jsonConfig, const std::string& key, const std::string& defaultColor);

    static void setColorToJson(nlohmann::json& jsonConfig, const std::string& key, const std::string& colorName);
};


#endif //MCLIB_COLORCONFIG_H
