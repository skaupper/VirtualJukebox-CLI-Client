#ifndef NETWORK_EXCEPTION_H
#define NETWORK_EXCEPTION_H

#include "Exception.h"


enum class NetworkExceptionCode : int {
    UNAUTHORIZED    = 401,
    FORBIDDEN       = 403,
    NOT_FOUND       = 404
    // TODO: to be extended
};


class NetworkException : public Exception {
public:
    NetworkException(NetworkExceptionCode code);
};

#endif
