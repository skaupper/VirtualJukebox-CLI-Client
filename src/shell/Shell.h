#ifndef SHELL_H
#define SHELL_H

#include "ShellCommand.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>


class ShellCommand;
using Commands = std::map<std::string, std::unique_ptr<ShellCommand>>;

class Shell {
public:
    Shell(std::string = "> ");
    void addCommand(const std::string &, std::unique_ptr<ShellCommand> &&);
    void handleInputs(std::istream &, std::ostream &);

private:
    const std::string mPrompt;
    Commands mCommands;
};


#endif
