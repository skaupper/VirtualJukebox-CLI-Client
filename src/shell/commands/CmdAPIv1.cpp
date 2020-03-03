#include "CmdAPIv1.h"

#include "exceptions/ShellException.h"
#include "exceptions/NetworkException.h"


static int strToInt(const std::string &valStr) {
    int valInt;
    size_t idx;

    try {
        valInt = stoi(valStr, &idx);
    } catch (const std::invalid_argument &) {
        throw ShellException(ShellExceptionCode::INVALID_ARGUMENTS);
    }

    if (idx != valStr.size()) {
        throw ShellException(ShellExceptionCode::INVALID_ARGUMENTS);
    }

    return valInt;
}


CmdAPIv1::CmdAPIv1(std::istream &in, const std::string &prompt) : ShellCommand(), mPrompt(prompt), mShell(mPrompt), mInput(in)
{
    api::v1::Initialize();
    // mShell.removeCommand("exit");
}

CmdAPIv1::~CmdAPIv1()
{
    api::v1::Deinitialize();
}


bool CmdAPIv1::execute(std::ostream &out, const std::vector<std::string> &arguments)
{
    if (arguments.size() < 3 || arguments.size() > 4) {
        throw ShellException(ShellExceptionCode::INVALID_ARGUMENTS);
    }

    std::string serverIp    = arguments[0];
    std::string serverPort  = arguments[1];
    std::string nickname    = arguments[2];

    mApi.setServerAddress(serverIp, strToInt(serverPort));

    out << "Try to connect to the server " << serverIp << ":" << serverPort << "..." << std::endl;

    try {
        if (arguments.size() == 4) {
            std::string password = arguments[3];
            mApi.generateAdminSession(nickname, password);
            out << "Successfully generated an admin session!" << std::endl;
        } else {
            mApi.generateSession(nickname);
            out << "Successfully generated a user session!"   << std::endl;
        }

        return runShell(out);
    } catch (const NetworkException &ex) {
        throw ShellException(ShellExceptionCode::NESTED, &ex);
    }
}

ShellCommandDetails CmdAPIv1::getCommandDetails() const
{
    ShellCommandDetails details;
    details.description = "Connect to a running VirtualJukebox server with the given credentials and start a subshell afterwards.";
    details.usage = getTrigger() + " <server_ip> <server_port> <nickname> [<admin_password>]";
    details.parameterDescription["<server_ip>"]       = "The IP of the VirtualJukebox server. May also be a valid hostname.";
    details.parameterDescription["<server_port>"]     = "The port of the VirtualJukebox server. If the value is not an integer, an exception is thrown.";
    details.parameterDescription["<nickname>"]        = "A non-unique nickname which is used when connecting to the server.";
    details.parameterDescription["<admin_password>"]  = "If the admin password is given, try to connect as an administrator (and fail if the password is wrong).";
    return details;
}

bool CmdAPIv1::runShell(std::ostream &out)
{
    mShell.handleInputs(mInput, out);
    return false;
}
