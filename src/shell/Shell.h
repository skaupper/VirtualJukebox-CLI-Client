#ifndef SHELL_H
#define SHELL_H

#include "ShellCommand.h"

#include <string>
#include <map>
#include <memory>
#include <iostream>


class ShellCommand;
typedef std::map<std::string, std::unique_ptr<ShellCommand>> Commands;

class Shell
{
public:
    Shell(const std::string & = "> ");
    void addCommand(const std::string &, std::unique_ptr<ShellCommand> &&);
    std::unique_ptr<ShellCommand> removeCommand(const std::string &);
    void handleInputs(std::istream &, std::ostream &);

private:
    const std::string mPrompt;
    Commands mCommands;
};


#endif
