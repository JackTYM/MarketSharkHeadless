#include "auth/Auth.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclibd.lib")
#else
#pragma comment(lib, "../Release/mclib.lib")
#endif


int main(void) {
    Auth::setupWebsocket();
}