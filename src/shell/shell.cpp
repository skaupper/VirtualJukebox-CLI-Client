#include "shell.h"


void Shell::addCommand(const std::string &commandTrigger, const ShellCommand &command)
{
    if (mCommand.find(commandTrigger) != mCommand.cend()) {
        throw ShellException(ShellExceptionCode::COMMAND_ALREADY_EXISTS);
    }
    mCommand[commandTrigger] = command;
}

void Shell::handleInputs(std::ostream &in, std::ostream &out)
{

}
