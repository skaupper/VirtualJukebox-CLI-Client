#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

#include "Shell.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>


struct ShellCommandDetails {
    std::string description                                 = "";
    std::string usage                                       = "";
    std::map<std::string, std::string> parameterDescription = {};
};


class Shell;

class ShellCommand {
    friend class Shell;


public:
    virtual ~ShellCommand()                                                = default;
    virtual bool execute(std::ostream &, const std::vector<std::string> &) = 0;
    virtual ShellCommandDetails getCommandDetails() const                  = 0;

protected:
    std::string getTrigger() const;

private:
    void setTrigger(const std::string &trigger);

    std::string mCommandTrigger;
};


#endif
