#include "Help.h"

#include "exceptions/ShellException.h"

#include <cassert>
#include <iostream>


static void listCommands(std::ostream &out, const Commands &commands) {
    for (const auto &[trigger, command] : commands) {
        out << trigger << ": " << command->getCommandDetails().description << std::endl;
    }
}

static void printCommandHelp(std::ostream &out, const std::string &command, const Commands &commands) {
    const auto commandIt {commands.find(command)};
    if (commandIt == commands.cend()) {
        throw ShellException(ShellExceptionCode::UNKNOWN_COMMAND);
    }

    const auto details {commandIt->second->getCommandDetails()};
    out << "Description : " << details.description << std::endl;
    out << "Usage       : " << details.usage << std::endl;
    for (const auto &[argument, desc] : details.parameterDescription) {
        out << argument << ": " << desc << std::endl;
    }
}


namespace commands {

    Help::Help(const Commands &commands) : mCommands(commands) {}


    void Help::doExecute(const std::vector<std::string> &arguments) {
        if (arguments.size() > 1) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
        }

        if (arguments.empty()) {
            listCommands(getOut(), mCommands);
        } else {
            printCommandHelp(getOut(), arguments[0], mCommands);
        }
    }

    ShellCommandDetails Help::getCommandDetails() const {
        ShellCommandDetails details;
        details.description = "Lists all available commands or prints the help text for one specific command.";
        details.usage       = getTrigger() + " [<command>]";
        details.parameterDescription["<command>"] = "The command whose help text should be printed.";
        return details;
    }

}  // namespace commands
