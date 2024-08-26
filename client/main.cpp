#include "auth/Auth.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclibd.lib")
#else
#pragma comment(lib, "../Release/mclib.lib")
#endif

#include <Colors.h>
#include <ColorConfig.h>

int main(void) {
    Objects::loadConfig();
    Objects::saveConfig();

    ColorConfig::loadColors();
    ColorConfig::saveColors();

    Logger::initialize();

    if (Objects::getKey() == "" ) {
        Logger::log(ColorConfig::Error + "Please put your MarketShark key into config.json!" + Colors::End);
        return 0;
    }
    Auth::setupWebsocket();
}