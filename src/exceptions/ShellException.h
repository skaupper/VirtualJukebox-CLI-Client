#ifndef SHELL_EXCEPTION_H
#define SHELL_EXCEPTION_H

#include "Exception.h"


enum class ShellExceptionCode : int {
    COMMAND_ALREADY_KNOWN
    // TODO: to be extended
};

class ShellException : public Exception {
public:
    ShellException(ShellExceptionCode code);
};


#endif
