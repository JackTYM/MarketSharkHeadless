#include <iostream>
#include <Colors.h>

const std::string Colors::End = "\033[0m\n";
const std::string Colors::ResetHighlight = Colors::Reset + Colors::BrightYellowBackground;

const std::string Colors::Reset = "\033[0m";
const std::string Colors::Bold = "\033[1m";
const std::string Colors::Underline = "\033[4m";
const std::string Colors::Inverse = "\033[7m";

const std::string Colors::Black = "\033[30m";
const std::string Colors::Red = "\033[31m";
const std::string Colors::Green = "\033[32m";
const std::string Colors::Yellow = "\033[33m";
const std::string Colors::Blue = "\033[34m";
const std::string Colors::Magenta = "\033[35m";
const std::string Colors::Cyan = "\033[36m";
const std::string Colors::White = "\033[37m";
const std::string Colors::Gray = "\033[90m";

const std::string Colors::BlackBackground = "\033[40m";
const std::string Colors::RedBackground = "\033[41m";
const std::string Colors::GreenBackground = "\033[42m";
const std::string Colors::YellowBackground = "\033[43m";
const std::string Colors::BlueBackground = "\033[44m";
const std::string Colors::MagentaBackground = "\033[45m";
const std::string Colors::CyanBackground = "\033[46m";
const std::string Colors::WhiteBackground = "\033[47m";
const std::string Colors::GrayBackground = "\033[100m";

const std::string Colors::BrightBlack = "\033[90m";
const std::string Colors::BrightRed = "\033[91m";
const std::string Colors::BrightGreen = "\033[92m";
const std::string Colors::BrightYellow = "\033[93m";
const std::string Colors::BrightBlue = "\033[94m";
const std::string Colors::BrightMagenta = "\033[95m";
const std::string Colors::BrightCyan = "\033[96m";
const std::string Colors::BrightWhite = "\033[97m";

const std::string Colors::BrightBlackBackground = "\033[100m";
const std::string Colors::BrightRedBackground = "\033[101m";
const std::string Colors::BrightGreenBackground = "\033[102m";
const std::string Colors::BrightYellowBackground = "\033[103m";
const std::string Colors::BrightBlueBackground = "\033[104m";
const std::string Colors::BrightMagentaBackground = "\033[105m";
const std::string Colors::BrightCyanBackground = "\033[106m";
const std::string Colors::BrightWhiteBackground = "\033[107m";

std::string Colors::convertColorCodes(std::string message) {
    std::unordered_map<char, std::string> colorMap = {
            {'0', Colors::Black},
            {'1', Colors::Blue},
            {'2', Colors::Green},
            {'3', Colors::Cyan},
            {'4', Colors::Red},
            {'5', Colors::Magenta},
            {'6', Colors::Yellow},
            {'7', Colors::White},
            {'8', Colors::Gray},
            {'9', Colors::BrightBlue},
            {'a', Colors::BrightGreen},
            {'b', Colors::BrightCyan},
            {'c', Colors::BrightRed},
            {'d', Colors::BrightMagenta},
            {'e', Colors::BrightYellow},
            {'f', Colors::BrightWhite},
            {'l', Colors::Bold},
            {'n', Colors::Underline},
            {'r', Colors::ResetHighlight}
    };

    std::string result;
    result.reserve(message.size());

    for (size_t i = 0; i < message.size(); ++i) {
        if (static_cast<unsigned char>(message[i]) == 0xC2 && i + 1 < message.size() &&
            static_cast<unsigned char>(message[i + 1]) == 0xA7) {
            char colorCode = message[i + 2];
            auto it = colorMap.find(colorCode);
            if (it != colorMap.end()) {
                result += it->second;
            }
            i += 2;
        } else {
            result += message[i];
        }
    }

    return result;
}

std::string Colors::stripColorCodes(std::string message) {
    std::size_t pos = message.find((char) 0xA7);
    while (pos != std::string::npos) {
        message.erase(pos, 2);
        pos = message.find((char) 0xA7);
    }

    // Remove any non-printable characters
    message.erase(std::remove_if(message.begin(), message.end(), [](char c) {
        return c < 32 || c > 126;
    }), message.end());
    return message;
};