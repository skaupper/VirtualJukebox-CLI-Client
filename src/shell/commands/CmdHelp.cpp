#include "CmdHelp.h"

#include "exceptions/ShellException.h"

#include <iostream>
#include <cassert>


static void listCommands(std::ostream &out, const Commands *commands)
{
    assert(commands);

    for (const auto &[trigger, command] : *commands) {
        out << trigger << ": " << command->getCommandDetails().description << std::endl;
    }
}

static void printCommandHelp(std::ostream &out, const std::string &command, const Commands *commands)
{
    assert(commands);

    auto commandIt = commands->find(command);
    if (commandIt == commands->cend()) {
        throw ShellException(ShellExceptionCode::UNKNOWN_COMMAND);
    }

    auto details = commandIt->second->getCommandDetails();
    out << command << ":" << std::endl;
    out << "Description : " << details.description << std::endl;
    out << "Usage       : " << details.usage << std::endl;
    for (auto &[argument, desc] : details.parameterDescription) {
        out << argument << ": " << desc << std::endl;
    }
}


CmdHelp::CmdHelp(const Commands &commands) : ShellCommand(), mCommands(&commands)
{
}


void CmdHelp::execute(std::ostream &out, const std::vector<std::string> &arguments)
{
    if (arguments.size() > 1) {
        throw ShellException(ShellExceptionCode::INVALID_ARGUMENTS);
    }

    if (arguments.size() == 0) {
        listCommands(out, mCommands);
    } else {
        printCommandHelp(out, arguments[0], mCommands);
    }
}

ShellCommandDetails CmdHelp::getCommandDetails() const
{
    ShellCommandDetails details;
    details.description = "Lists all available commands or prints the help text for one specific command.";
    details.usage = getTrigger() + " [command]";
    details.parameterDescription["command"] = "The command whose help text should be printed.";
    return details;
}
