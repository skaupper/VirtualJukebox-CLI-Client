#ifndef SHELL_EXCEPTION_H
#define SHELL_EXCEPTION_H

#include "Exception.h"


enum class ShellExceptionCode : int {
    UNKNOWN_COMMAND,
    COMMAND_CONFIGURATION,
    COMMAND_ALREADY_EXISTS,
    INVALID_ARGUMENT_NUMBER,
    INVALID_ARGUMENT_FORMAT,
    INVALID_ARGUMENT_VALUE
    // TODO: to be extended
};

class ShellException : public Exception {
    ShellExceptionCode mCode;

public:
    explicit ShellException(ShellExceptionCode code);
    ShellExceptionCode getCode() const;
};


#endif
