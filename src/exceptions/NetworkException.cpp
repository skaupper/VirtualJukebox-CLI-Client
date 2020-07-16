#include "NetworkException.h"

#include <sstream>


static std::string mapExceptionCodeToString(NetworkExceptionCode code) {
    switch (code) {
    case NetworkExceptionCode::BAD_REQUEST:
        return "Bad request";
    case NetworkExceptionCode::UNAUTHORIZED:
        return "Unauthorized access";
    case NetworkExceptionCode::FORBIDDEN:
        return "Access forbidden";
    case NetworkExceptionCode::NOT_FOUND:
        return "Resource not found";
    case NetworkExceptionCode::UNPROCESSABLE_ENTITY:
        return "Unprocessable entity";
    case NetworkExceptionCode::LOGIN_TIME_OUT:
        return "Login timeout";
    case NetworkExceptionCode::INTERNAL_SERVER_ERROR:
        return "Internal server error";
    case NetworkExceptionCode::BAD_GATEWAY:
        return "Bad gateway";

    default:
        return "Unknown error (" + std::to_string(static_cast<int>(code)) + ")";
    }
}


NetworkException::NetworkException(NetworkExceptionCode code) {
    std::stringstream msg;
    msg << "A network error occurred: ";
    msg << mapExceptionCodeToString(code);
    setMsg(msg.str());
}
