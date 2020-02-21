#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <map>


class ShellCommand
{
public:
    virtual void execute(std::ostream &) = 0;
    virtual void printHelp(std::ostream &) const = 0;
};

class Shell
{
public:
    void addCommand(const std::string &, const ShellCommand &);
    void handleInputs(std::ostream &, std::ostream &);

private:
    std::map<std::string, ShellCommand> mCommands;
};


#endif
