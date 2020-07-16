#ifndef CMD_EXIT_H
#define CMD_EXIT_H

#include "shell/ShellCommand.h"


class CmdExit : public ShellCommand {
public:
    bool execute(std::ostream &, const std::vector<std::string> &) override;
    ShellCommandDetails getCommandDetails() const override;
};


#endif
