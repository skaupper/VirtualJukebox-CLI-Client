#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <variant>
#include <stdexcept>




enum class NetworkExceptionCode : int {
    UNAUTHORIZED    = 401,
    FORBIDDEN       = 403,
    NOT_FOUND       = 404
    // TODO: to be extended
};

// TODO: put this class in a separate file
class NetworkException : std::runtime_error {
    NetworkExceptionCode code;

    mutable std::string msg;

public:
    NetworkException(NetworkExceptionCode code) : std::runtime_error(""), code(code) {}
    const char *what() const noexcept override {
        msg = "An network error occurred: " + std::to_string(static_cast<int>(code));
        return msg.c_str();
    }
};



// TODO: put this class in a separate file
class InvalidFormatException : std::runtime_error {
    std::string desc;
    std::string invalidData;

    mutable std::string msg;

public:
    InvalidFormatException(const std::string &desc, const std::string &invalidData) : std::runtime_error(""), desc(desc), invalidData(invalidData) {}
    const char *what() const noexcept override {
        msg = desc + ": \"" + invalidData + "\"";
        return msg.c_str();
    }
};


enum class APIExceptionCode : int {
    NO_SESSION_GENERATED
    // TODO: to be extended
};

// TODO: put this class in a separate file
class APIException : std::runtime_error {
    APIExceptionCode code;

    mutable std::string msg;

public:
    APIException(APIExceptionCode code) : std::runtime_error(""), code(code) {}
    const char *what() const noexcept override {
        msg = "An API error occurred: " + std::to_string(static_cast<int>(code));
        return msg.c_str();
    }
};



#endif
