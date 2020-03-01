#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include <map>
#include <memory>


struct ShellCommandDetails
{
    std::string description                                 = "";
    std::string usage                                       = "";
    std::map<std::string, std::string> parameterDescription = {};
};

class ShellCommand
{
    friend class Shell;


public:
    virtual void execute(std::ostream &, const std::vector<std::string> &) = 0;
    virtual ShellCommandDetails getCommandDetails() const = 0;

protected:
    std::string getTrigger() const;

private:
    void setTrigger(const std::string &trigger);

    std::string mCommandTrigger;
};


typedef std::map<std::string, std::unique_ptr<ShellCommand>> Commands;

class Shell
{
public:
    Shell();
    void addCommand(const std::string &, std::unique_ptr<ShellCommand> &&);
    void handleInputs(std::istream &, std::ostream &);

private:
    Commands mCommands;
};


#endif
