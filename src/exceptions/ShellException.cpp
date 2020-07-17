#include "ShellException.h"

#include <sstream>


static std::string mapExceptionCodeToString(ShellExceptionCode code) {
    switch (code) {
    case ShellExceptionCode::UNKNOWN_COMMAND:
        return "The given command has not been found.";

    case ShellExceptionCode::COMMAND_CONFIGURATION:
        return "The command has not been configured appropriately.";

    case ShellExceptionCode::COMMAND_ALREADY_EXISTS:
        return "A command with the given name already exists.";

    case ShellExceptionCode::INVALID_ARGUMENT_NUMBER:
        return "The arguments for the given command are invalid.";

    case ShellExceptionCode::INVALID_ARGUMENT_FORMAT:
        return "An argument of the given command has invalid format.";

    default:
        return "Unknown error (" + std::to_string(static_cast<int>(code)) + ")";
    }
}


ShellException::ShellException(ShellExceptionCode code) : mCode(code) {
    std::stringstream msg;
    msg << mapExceptionCodeToString(code);
    setMsg(msg.str());
}

ShellExceptionCode ShellException::getCode() const { return mCode; }
