#include "Exit.h"


namespace commands {

    void Exit::doExecute(const std::vector<std::string> & /*args*/) { closeShell(); }

    ShellCommandDetails Exit::getCommandDetails() const {
        ShellCommandDetails details;
        details.description          = "Exits from the current shell.";
        details.usage                = "exit";
        details.parameterDescription = {};
        return details;
    }

}  // namespace commands
