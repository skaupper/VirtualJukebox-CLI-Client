#ifndef INVALID_FORMAT_EXCEPTION_H
#define INVALID_FORMAT_EXCEPTION_H

#include "Exception.h"


class InvalidFormatException : public Exception {
public:
    InvalidFormatException(const std::string &desc, const std::string &invalidData);
};

#endif
