#ifndef CMD_APIV1_H
#define CMD_APIV1_H

#include "api/api-v1.h"
#include "shell/ShellCommand.h"


class CmdAPIv1 : public ShellCommand {
public:
    CmdAPIv1(std::istream &, const std::string & = "ApiV1> ");
    ~CmdAPIv1();

    bool execute(std::ostream &, const std::vector<std::string> &) override;
    ShellCommandDetails getCommandDetails() const override;

private:
    bool runShell(std::ostream &);

    const std::string mPrompt;
    api::v1::APIv1 mApi;
    Shell mShell;
    std::istream &mInput;
};


#endif
