#include "ShellException.h"

#include <cassert>
#include <sstream>


static std::string mapExceptionCodeToString(ShellExceptionCode code) {
    switch (code) {
    case ShellExceptionCode::UNKNOWN_COMMAND:
        return "The given command has not been found.";

    case ShellExceptionCode::COMMAND_ALREADY_EXISTS:
        return "A command with the given name already exists.";

    case ShellExceptionCode::INVALID_ARGUMENTS:
        return "The arguments for the given command are invalid.";

    case ShellExceptionCode::NESTED:  // Nested exceptions get handled separately
    default:
        return "Unknown error (" + std::to_string(static_cast<int>(code)) + ")";
    }
}


ShellException::ShellException(ShellExceptionCode code, const Exception *nestedException)
    : mCode(code), mNestedException(nestedException) {
    if (code == ShellExceptionCode::NESTED) {
        assert(mNestedException);
        setMsg(mNestedException->what());
    } else {
        std::stringstream msg;
        msg << "A shell error occurred: ";
        msg << mapExceptionCodeToString(code);
        setMsg(msg.str());
    }
}

ShellExceptionCode ShellException::getCode() const {
    return mCode;
}
