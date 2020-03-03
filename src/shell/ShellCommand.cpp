#include "ShellCommand.h"


std::string ShellCommand::getTrigger() const
{
    return mCommandTrigger;
}

void ShellCommand::setTrigger(const std::string &trigger)
{
    mCommandTrigger = trigger;
}
