#include "api/api-v1.h"
#include "shell/Shell.h"
#include "shell/commands/APIv1Commands.h"

#include "Exception.h"

#include <spdlog/spdlog.h>

#include <cassert>
#include <iostream>


int main() {
    spdlog::set_level(spdlog::level::debug);

    Shell shell("VirtualJukebox> ");
    shell.addCommand("login", std::make_unique<commands::Login>());
    shell.addCommand("print", std::make_unique<commands::PrintQueues>());
    shell.handleInputs(std::cin, std::cout);


    // constexpr auto SERVER_ADDRESS {"skaupper.tk"};
    // constexpr auto SERVER_PORT {8888};
    // constexpr auto NICKNAME {"testuser"};
    // constexpr auto ADMIN_PASSWORD {"admin"};

    // try {
    //     api::v1::APIv1 api(SERVER_ADDRESS, SERVER_PORT);
    //     api.generateAdminSession(NICKNAME, ADMIN_PASSWORD);

    //     std::cout << "Session ID: " << api.getSessionId() << std::endl;
    //     std::cout << "IsAdminSession: " << std::boolalpha << api.isAdmin() << std::endl;
    //     std::cout << std::endl;


    //     const auto tracks = api.queryTracks("offspring", 2);
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
