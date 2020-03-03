#include "Shell.h"

#include "commands/CmdHelp.h"
#include "commands/CmdExit.h"
#include "exceptions/ShellException.h"

#include <iostream>
#include <sstream>



Shell::Shell(const std::string &prompt) : mPrompt(prompt)
{
    addCommand("help", std::make_unique<CmdHelp>(CmdHelp(mCommands)));
    addCommand("exit", std::make_unique<CmdExit>());
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
    std::string line;

    bool exit = false;
    while (!exit) {
        //
        // Print prompt and wait for input
        //
        out << mPrompt;
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
        while (!std::getline(lineStream, arg, ' ').fail()) {
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
            exit = commandIt->second->execute(out, arguments);
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
