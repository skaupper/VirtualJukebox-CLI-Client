#ifndef SHELL_EXCEPTION_H
#define SHELL_EXCEPTION_H

#include "Exception.h"


enum class ShellExceptionCode : int {
    UNKNOWN_COMMAND,
    COMMAND_ALREADY_EXISTS,
    INVALID_ARGUMENTS,
    NESTED
    // TODO: to be extended
};

class ShellException : public Exception {
    ShellExceptionCode mCode;
    const Exception *mNestedException = nullptr;

public:
    ShellException(ShellExceptionCode code, const Exception *nestedException = nullptr);
    ShellExceptionCode getCode() const;
};


#endif
