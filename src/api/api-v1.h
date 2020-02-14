#ifndef API_V1_H
#define API_V1_H

#include "types.h"


class APIv1
{
    std::string address;
    int port;

    std::string sessionId;
    bool isAdmin;
    bool isSessionGenerated = false;

public:
    void setServerAddress(const std::string &addres, int port);

    void generateSession(const std::string &nickname);
    void generateAdminSession(const std::string &nickname, const std::string &adminPassword);
};

#endif
