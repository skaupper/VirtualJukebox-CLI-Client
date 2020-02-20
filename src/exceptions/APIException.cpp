#include "APIException.h"

#include <sstream>


static std::string mapExceptionCodeToString(APIExceptionCode code)
{
    switch(code)
    {
        case APIExceptionCode::NO_SESSION_GENERATED:
            return "No session has been generated yet.";
        case APIExceptionCode::INVALID_PASSWORD:
            return "The given password is invalid.";
        case APIExceptionCode::ADMIN_REQUIRED:
            return "The requested operation needs admin privileges.";
        case APIExceptionCode::UNKNOWN_ENUM_VARIANT:
            return "An unknown enum variant has been encountered.";
        case APIExceptionCode::NOT_IMPLEMENTED:
            return "The requested endpoint is not implemented yet.";

        default:
            return "Unknown error (" + std::to_string(static_cast<int>(code)) + ")";
    }
}


APIException::APIException(APIExceptionCode code)
{
    std::stringstream msg;
    msg << "An API error occurred: ";
    msg << mapExceptionCodeToString(code);
    setMsg(msg.str());
}
