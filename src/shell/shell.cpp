#include "shell.h"

#include "commands/CmdHelp.h"
#include "exceptions/ShellException.h"

#include <iostream>
#include <sstream>




std::string ShellCommand::getTrigger() const
{
    return mCommandTrigger;
}

void ShellCommand::setTrigger(const std::string &trigger)
{
    mCommandTrigger = trigger;
}




Shell::Shell()
{
    addCommand("help", std::make_unique<CmdHelp>(CmdHelp(mCommands)));
}


void Shell::addCommand(const std::string &commandTrigger, std::unique_ptr<ShellCommand> &&command)
{
    if (mCommands.find(commandTrigger) != mCommands.cend()) {
        throw ShellException(ShellExceptionCode::COMMAND_ALREADY_EXISTS);
    }
    command->setTrigger(commandTrigger);
    mCommands[commandTrigger] = std::move(command);
}

void Shell::handleInputs(std::istream &in, std::ostream &out)
{
    static const std::string PROMPT = "> ";
    std::string line;

    while (true) {
        //
        // Print prompt and wait for input
        //
        out << PROMPT;
        if (!std::getline(in, line).good()) {
            break;
        }


        //
        // Split line into command and arguments
        //
        std::string arg;
        std::string command;
        std::vector<std::string> arguments;
        bool first = true;

        std::stringstream lineStream(line);
        while (std::getline(lineStream, arg).good()) {
            if (first) {
                command = arg;
                first = false;
            } else {
                arguments.push_back(arg);
            }
        }


        //
        // Check if the command is known
        //
        auto commandIt = mCommands.find(command);
        if (commandIt == mCommands.cend()) {
            out << ShellException(ShellExceptionCode::UNKNOWN_COMMAND).what() << std::endl;
            continue;
        }


        //
        // Execute command
        //
        try {
            commandIt->second->execute(out, arguments);
        } catch (const ShellException &ex) {
            switch (ex.getCode()) {
                case ShellExceptionCode::INVALID_ARGUMENTS:
                    out << ex.what() << std::endl;
                    out << "Try 'help " << command << "' for further information." << std::endl;
                    break;

                default:
                    throw;
            }
        }
    }
}
