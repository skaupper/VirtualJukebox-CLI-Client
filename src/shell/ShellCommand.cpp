#include "ShellCommand.h"

#include "exceptions/ShellException.h"


bool ShellCommand::execute(const std::vector<std::string> &args) {

    if (!mOutStream || !mInStream) {
        throw ShellException(ShellExceptionCode::COMMAND_CONFIGURATION);
    }

    doExecute(args);
    return mCloseShell;
}

std::string ShellCommand::getTrigger() const { return mCommandTrigger; }
std::ostream &ShellCommand::getOut() { return *mOutStream; }
std::istream &ShellCommand::getIn() { return *mInStream; }

void ShellCommand::closeShell() { mCloseShell = true; }

void ShellCommand::configure(std::ostream &out, std::istream &in, const std::string &trigger) {
    mOutStream      = &out;
    mInStream       = &in;
    mCommandTrigger = trigger;
}
