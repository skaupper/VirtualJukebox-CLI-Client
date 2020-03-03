#include "api/api-v1.h"
#include "shell/Shell.h"

#include "Exception.h"

#include <iostream>
#include <cassert>


int main()
{
    Shell shell;
    shell.handleInputs(std::cin, std::cout);


    // static const std::string SERVER_ADDRESS = "skaupper.tk";
    // static const int         SERVER_PORT    = 8888;
    // static const std::string NICKNAME       = "testuser";
    // static const std::string ADMIN_PASSWORD = "admin";

    // try {
    //     api::v1::APIv1 api;
    //     api.setServerAddress(SERVER_ADDRESS, SERVER_PORT);
    //     api.generateAdminSession(NICKNAME, ADMIN_PASSWORD);

    //     std::cout << "Session ID: " << api.getSessionId() << std::endl;
    //     std::cout << "IsAdminSession: " << std::boolalpha << api.isAdmin() << std::endl;
    //     std::cout << std::endl;


    //     const auto tracks = api.queryTracks("linkin park", 10);
    //     for (const auto &t : tracks) {
    //         api.addTrack(t);
    //     }
    //     const auto queue = api.getCurrentQueues();

    //     if (queue.currentlyPlaying.has_value()) {
    //         std::cout << "Currently playing: " << queue.currentlyPlaying.value().title << std::endl;
    //     } else {
    //         std::cout << "No track is currently playing" << std::endl;
    //     }

    // } catch (const Exception &ex) {
    //     std::cerr << ex.what() << std::endl;
    //     return -1;
    // }

    return 0;
}
