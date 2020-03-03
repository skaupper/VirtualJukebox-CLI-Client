#include "Exception.h"


void Exception::setMsg(const std::string &msg) noexcept {
    mMsg = msg;
}

const char *Exception::what() const noexcept {
    return mMsg.c_str();
}
