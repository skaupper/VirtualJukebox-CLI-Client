#ifndef CMD_HELP_H
#define CMD_HELP_H

#include "shell/ShellCommand.h"


namespace commands {

    class Help : public ShellCommand {
        friend class ::Shell;

    public:
        ShellCommandDetails getCommandDetails() const override;

    protected:
        void doExecute(const std::vector<std::string> &) override;

    private:
        explicit Help(const Commands &);
        const Commands &mCommands;
    };

}  // namespace commands

#endif
