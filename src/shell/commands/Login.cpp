#include "APIv1Commands.h"

#include "utils/utils.h"

#include "api/api-v1-provider.h"
#include "exceptions/ShellException.h"


//
// Helper functions
//

static auto parseArgs(const std::vector<std::string> &args) {

    std::string address {args[0]};
    auto port {sk::to_number<unsigned int>(args[1])};

    if (!port) {
        throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_FORMAT);
    }

    std::optional<std::string> nickname;
    std::optional<std::string> adminPassword;


    if (std::size(args) > 2) {
        nickname = args[2];

        if (std::size(args) > 3) {
            adminPassword = args[3];
        }
    }

    return std::tuple {address, port.value(), nickname, adminPassword};
}


//
// Actual command
//

namespace commands {

    bool Login::execute(std::ostream &out, const std::vector<std::string> &args) {

        if (std::size(args) < 2 || std::size(args) > 4) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
        }


        const auto [address, port, nickname, adminPassword] = parseArgs(args);

        auto api = api::v1::createNewApi(address, port);
        if (adminPassword) {
            api->generateAdminSession(adminPassword.value(), nickname);
            out << "Successfully logged in as admin." << std::endl;
        } else {
            api->generateSession(nickname);
            out << "Successfully logged." << std::endl;
        }

        return false;
    }

    ShellCommandDetails Login::getCommandDetails() const {
        ShellCommandDetails details;
        details.description                        = "Logs into a VirtualJukebox server.";
        details.usage                              = getTrigger() + " <address> <port> [<nickname> [<admin_password]]";
        details.parameterDescription["<address>"]  = "The IP address or the hostname of the server.";
        details.parameterDescription["<port>"]     = "The port the server is running on.";
        details.parameterDescription["<nickname>"] = "The non-unique name to be displayed on clients.";
        details.parameterDescription["<admin_password>"] =
            "If an admin session is desired, the admin password can be provided.";
        return details;
    }

}  // namespace commands
