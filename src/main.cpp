

#include "api/v1/Api.h"
#include "shell/Shell.h"
#include "shell/commands/v1/ApiCommands.h"

#include "Exception.h"

#include <spdlog/spdlog.h>

#include <cassert>
#include <iostream>

#include <sstream>


int main() {
    spdlog::set_level(spdlog::level::off);

    Shell shell("VirtualJukebox> ");
    shell.addCommand("login", std::make_unique<commands::v1::Login>());
    shell.addCommand("print", std::make_unique<commands::v1::PrintQueues>());
    shell.addCommand("addtrack", std::make_unique<commands::v1::AddTrack>());
    shell.addCommand("play", std::make_unique<commands::v1::Play>());
    shell.addCommand("pause", std::make_unique<commands::v1::Pause>());
    shell.addCommand("skip", std::make_unique<commands::v1::Skip>());
    shell.addCommand("volume", std::make_unique<commands::v1::Volume>());
    shell.handleInputs(std::cin, std::cout);

    return 0;
}
