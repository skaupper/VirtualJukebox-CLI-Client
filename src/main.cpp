#include "api/api-v1.h"

#include "Exception.h"

#include <iostream>
#include <cassert>


int main()
{
    static const std::string SERVER_ADDRESS = "skaupper.tk";
    static const int         SERVER_PORT    = 8888;
    static const std::string NICKNAME       = "testuser";
    static const std::string ADMIN_PASSWORD = "admisn";

    try {
        APIv1 api;
        api.setServerAddress(SERVER_ADDRESS, SERVER_PORT);
        api.generateAdminSession(NICKNAME, ADMIN_PASSWORD);

        std::cout << "Session ID: " << api.getSessionId() << std::endl;
        std::cout << "IsAdminSession: " << std::boolalpha << api.isAdmin() << std::endl;
    } catch (const Exception &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
