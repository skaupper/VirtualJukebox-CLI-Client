#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

#include "Shell.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>


struct ShellCommandDetails {
    std::string description;
    std::string usage;
    std::map<std::string, std::string> parameterDescription;
};


class Shell;

class ShellCommand {
    friend class Shell;


public:
    virtual ~ShellCommand() = default;

    bool execute(const std::vector<std::string> &);
    virtual ShellCommandDetails getCommandDetails() const = 0;

protected:
    std::string getTrigger() const;
    std::ostream &getOut();
    std::istream &getIn();

    void closeShell();

    virtual void doExecute(const std::vector<std::string> &) = 0;

private:
    void configure(std::ostream &, std::istream &, const std::string &trigger);

    bool mCloseShell = false;
    std::string mCommandTrigger;
    std::ostream *mOutStream = nullptr;
    std::istream *mInStream  = nullptr;
};


#endif
