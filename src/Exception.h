#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>


class Exception : public std::exception {
    std::string mMsg;

protected:
    void setMsg(const std::string &msg) noexcept;

public:
    const char *what() const noexcept override;
};

#endif
