#include "CmdExit.h"


bool CmdExit::execute(std::ostream & /*out*/, const std::vector<std::string> & /*args*/) { return true; }

ShellCommandDetails CmdExit::getCommandDetails() const {
    ShellCommandDetails details;
    details.description          = "Exits from the current shell.";
    details.usage                = "exit";
    details.parameterDescription = {};
    return details;
}
