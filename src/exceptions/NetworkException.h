#ifndef NETWORK_EXCEPTION_H
#define NETWORK_EXCEPTION_H

#include "Exception.h"


enum class NetworkExceptionCode : int {
    BAD_REQUEST           = 400,
    UNAUTHORIZED          = 401,
    FORBIDDEN             = 403,
    NOT_FOUND             = 404,
    UNPROCESSABLE_ENTITY  = 422,
    LOGIN_TIME_OUT        = 440,
    INTERNAL_SERVER_ERROR = 500,
    BAD_GATEWAY           = 502
    // TODO: to be extended
};


class NetworkException : public Exception {
public:
    NetworkException(NetworkExceptionCode code);
};

#endif
