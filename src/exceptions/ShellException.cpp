#include "APIException.h"

#include <sstream>


static std::string mapExceptionCodeToString(ShellExceptionCode code)
{
    switch(code)
    {
        case ShellExceptionCode::COMMAND_ALREADY_KNOWN:
            return "A command with the given name is already known.";

        default:
            return "Unknown error (" + std::to_string(static_cast<int>(code)) + ")";
    }
}


ShellException::ShellException(ShellExceptionCode code)
{
    std::stringstream msg;
    msg << "A shell error occurred: ";
    msg << mapExceptionCodeToString(code);
    setMsg(msg.str());
}
