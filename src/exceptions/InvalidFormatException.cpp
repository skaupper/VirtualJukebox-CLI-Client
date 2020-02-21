#include "InvalidFormatException.h"

#include <sstream>


InvalidFormatException::InvalidFormatException(const std::string &desc, const std::string &invalidData)
{
    std::stringstream msg;
    msg << "Invalid data format: " << desc << "." << std::endl;
    msg << invalidData;
    setMsg(msg.str());
}
