#ifndef CMD_EXIT_H
#define CMD_EXIT_H

#include "shell/ShellCommand.h"


namespace commands {

    class Exit : public ShellCommand {
    public:
        ShellCommandDetails getCommandDetails() const override;

    protected:
        void doExecute(const std::vector<std::string> &) override;
    };

}  // namespace commands


#endif
