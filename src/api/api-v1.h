#ifndef API_V1_H
#define API_V1_H

#include <string>


class APIv1
{
    std::string mAddress;
    int mPort;

    std::string mSessionId;
    bool mIsAdmin;
    bool mIsSessionGenerated = false;

public:
    void setServerAddress(const std::string &address, int port) noexcept;

    std::string getSessionId() const;

    bool isAdmin() const noexcept;
    bool isSessionGenerated() const noexcept;


    void generateSession(const std::string &nickname);
    void generateAdminSession(const std::string &nickname, const std::string &adminPassword);
};

#endif
