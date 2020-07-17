#ifndef CMD_APIV1_H
#define CMD_APIV1_H

#include "shell/ShellCommand.h"


namespace commands {

#define DECLARE_COMMAND(cmd_name)                                                                                      \
    class cmd_name : public ShellCommand {                                                                             \
    public:                                                                                                            \
        bool execute(std::ostream &, const std::vector<std::string> &) override;                                       \
        ShellCommandDetails getCommandDetails() const override;                                                        \
    }


    DECLARE_COMMAND(Login);
    DECLARE_COMMAND(PrintQueues);


#undef DECLARE_COMMAND

}  // namespace commands

#endif
