#include "api/api-v1.h"

#include <iostream>
#include <cassert>


int main()
{
    static const std::string SERVER_ADDRESS = "skaupper.tk";
    static const int         SERVER_PORT    = 8888;
    static const std::string NICKNAME       = "testuser";

    try {
        APIv1 api;
        api.setServerAddress(SERVER_ADDRESS, SERVER_PORT);
        api.generateSession(NICKNAME);

        assert(!api.isAdmin());
        assert(api.isSessionGenerated());

        std::cout << "Session ID: " << api.getSessionId() << std::endl;
    } catch (const NetworkException &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
