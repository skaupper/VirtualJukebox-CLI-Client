#include "Exit.h"


namespace commands {

    bool Exit::execute(std::ostream & /*out*/, const std::vector<std::string> & /*args*/) { return true; }

    ShellCommandDetails Exit::getCommandDetails() const {
        ShellCommandDetails details;
        details.description          = "Exits from the current shell.";
        details.usage                = "exit";
        details.parameterDescription = {};
        return details;
    }

}  // namespace commands
