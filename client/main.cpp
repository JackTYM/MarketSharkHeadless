#include "auth/Auth.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclibd.lib")
#else
#pragma comment(lib, "../Release/mclib.lib")
#endif

#include <Colors.h>

int main(void) {
    Objects::loadConfig();
    Objects::saveConfig();
    if (Objects::getKey() == "" ) {
        std::cout << Colors::Red << "Please put your MarketShark key into config.json!" << Colors::End;
        return 0;
    }
    Auth::setupWebsocket();
}