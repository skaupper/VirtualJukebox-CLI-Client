#ifndef CMD_HELP_H
#define CMD_HELP_H

#include "shell/ShellCommand.h"


class CmdHelp : public ShellCommand {
    friend class Shell;

public:
    bool execute(std::ostream &, const std::vector<std::string> &) override;
    ShellCommandDetails getCommandDetails() const override;

private:
    CmdHelp(const Commands &);
    const Commands *mCommands;
};


#endif
