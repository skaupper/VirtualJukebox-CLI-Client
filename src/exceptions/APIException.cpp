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

        default:
            return "Unknown error";
    }
}


APIException::APIException(APIExceptionCode code)
{
    std::stringstream msg;
    msg << "An API error occurred: ";
    msg << mapExceptionCodeToString(code);
    setMsg(msg.str());
}
