#ifndef CMD_APIV1_H
#define CMD_APIV1_H

#include "shell/ShellCommand.h"


namespace commands::v1 {

#define DECLARE_COMMAND(cmd_name)                                                                                      \
    class cmd_name : public ShellCommand {                                                                             \
    public:                                                                                                            \
        ShellCommandDetails getCommandDetails() const override;                                                        \
                                                                                                                       \
    protected:                                                                                                         \
        void doExecute(const std::vector<std::string> &) override;                                                     \
    }


    DECLARE_COMMAND(Login);
    DECLARE_COMMAND(PrintQueues);
    DECLARE_COMMAND(AddTrack);


#undef DECLARE_COMMAND

}  // namespace commands::v1

#endif
