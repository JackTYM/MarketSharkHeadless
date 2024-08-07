#include "Logger.h"

#include <mclib/common/Common.h>
#include <mclib/core/Client.h>
#include <mclib/util/Forge.h>
#include <mclib/util/Hash.h>
#include <mclib/util/Utility.h>
#include <mclib/util/VersionFetcher.h>

#include <iostream>

#include "auth/Auth.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclibd.lib")
#else
#pragma comment(lib, "../Release/mclib.lib")
#endif

namespace {

const std::string server("127.0.0.1");
const u16 port = 25565;
//const std::string username("testplayer");
//const std::string password("");
//const bool useProfileToken = false;

} // ns

int main(void) {
    Auth::setupWebsocket();
}