#ifndef CMD_EXIT_H
#define CMD_EXIT_H

#include "shell/ShellCommand.h"


namespace commands {

    class Exit : public ShellCommand {
    public:
        bool execute(std::ostream &, const std::vector<std::string> &) override;
        ShellCommandDetails getCommandDetails() const override;
    };

}  // namespace commands


#endif
