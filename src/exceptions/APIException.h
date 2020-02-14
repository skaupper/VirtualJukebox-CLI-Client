#ifndef API_EXCEPTION_H
#define API_EXCEPTION_H

#include "Exception.h"


enum class APIExceptionCode : int {
    NO_SESSION_GENERATED,
    INVALID_PASSWORD
    // TODO: to be extended
};

class APIException : public Exception {
public:
    APIException(APIExceptionCode code);
};


#endif
