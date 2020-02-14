#include "NetworkException.h"

#include <sstream>


static std::string mapExceptionCodeToString(NetworkExceptionCode code)
{
    switch(code)
    {
        case NetworkExceptionCode::UNAUTHORIZED:
            return "Unauthorized access";
        case NetworkExceptionCode::FORBIDDEN:
            return "Access forbidden";
        case NetworkExceptionCode::NOT_FOUND:
            return "Resource not found";

        default:
            return "Unknown error";
    }
}


NetworkException::NetworkException(NetworkExceptionCode code)
{
    std::stringstream msg;
    msg << "A network error occurred: ";
    msg << mapExceptionCodeToString(code);
    setMsg(msg.str());
}
