#include "CmdExit.h"


bool CmdExit::execute(std::ostream &, const std::vector<std::string> &) {
    return true;
}

ShellCommandDetails CmdExit::getCommandDetails() const {
    ShellCommandDetails details;
    details.description          = "Exits from the current shell.";
    details.usage                = getTrigger();
    details.parameterDescription = {};
    return details;
}
