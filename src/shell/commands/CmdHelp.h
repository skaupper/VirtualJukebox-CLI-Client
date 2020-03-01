#ifndef CMD_HELP_H
#define CMD_HELP_H

#include "shell/shell.h"


class CmdHelp : public ShellCommand
{
    friend class Shell;

public:
    void execute(std::ostream &, const std::vector<std::string> &) override;
    ShellCommandDetails getCommandDetails() const override;

private:
    CmdHelp(const Commands &);
    const Commands *mCommands;
};


#endif
