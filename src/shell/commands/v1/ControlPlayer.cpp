#include "ApiCommands.h"

#include "utils/utils.h"

#include "api/v1/Api.h"
#include "exceptions/ShellException.h"


using namespace api::v1;


//
// Helper functions
//


//
// Actual command
//

namespace commands::v1 {

    void Play::doExecute(const std::vector<std::string> &args) {

        if (std::size(args) != 0) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
        }

        auto api = api::v1::Api::getInstance();
        api->controlPlayer(PlayerAction::PLAY);
    }

    void Pause::doExecute(const std::vector<std::string> &args) {

        if (std::size(args) != 0) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
        }

        auto api = api::v1::Api::getInstance();
        api->controlPlayer(PlayerAction::PAUSE);
    }

    void Skip::doExecute(const std::vector<std::string> &args) {

        if (std::size(args) != 0) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
        }

        auto api = api::v1::Api::getInstance();
        api->controlPlayer(PlayerAction::SKIP);
    }

    void Volume::doExecute(const std::vector<std::string> &args) {

        if (std::size(args) != 1) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
        }

        auto api = api::v1::Api::getInstance();

        const std::string dir {args[0]};
        if (dir == "up") {
            api->controlPlayer(PlayerAction::VOLUME_UP);
        } else if (dir == "down") {
            api->controlPlayer(PlayerAction::VOLUME_DOWN);
        } else {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_VALUE);
        }
    }


    ShellCommandDetails Play::getCommandDetails() const {
        ShellCommandDetails details;
        details.description          = "Continue playback of the current track.";
        details.usage                = getTrigger();
        details.parameterDescription = {};
        return details;
    }

    ShellCommandDetails Pause::getCommandDetails() const {
        ShellCommandDetails details;
        details.description          = "Pause playback of the current track.";
        details.usage                = getTrigger();
        details.parameterDescription = {};
        return details;
    }

    ShellCommandDetails Skip::getCommandDetails() const {
        ShellCommandDetails details;
        details.description          = "Skip the currently playing track.";
        details.usage                = getTrigger();
        details.parameterDescription = {};
        return details;
    }

    ShellCommandDetails Volume::getCommandDetails() const {
        ShellCommandDetails details;
        details.description                         = "Changes the volume of the player.";
        details.usage                               = getTrigger() + " <direction>";
        details.parameterDescription["<direction>"] = "The direction of the volume change. Valid values are: up/down.";
        return details;
    }

}  // namespace commands::v1
