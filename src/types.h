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

public:
    NetworkException(NetworkExceptionCode code) : std::runtime_error(""), code(code) {}
    const char *what() const noexcept override {
        std::string msg = "An network error occurred: " + std::to_string(static_cast<int>(code));
        return msg.c_str();
    }
};



// TODO: put this class in a separate file
class InvalidFormatException : std::runtime_error {
    std::string desc;
    std::string invalidData;

public:
    InvalidFormatException(const std::string &desc, const std::string &invalidData) : std::runtime_error(""), desc(desc), invalidData(invalidData) {}
    const char *what() const noexcept override {
        std::string msg = desc + ": \"" + invalidData + "\"";
        return msg.c_str();
    }
};


#endif
