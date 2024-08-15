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
