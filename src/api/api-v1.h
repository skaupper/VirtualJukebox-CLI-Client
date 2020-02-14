#ifndef API_V1_H
#define API_V1_H

#include <string>
#include <vector>


struct Track
{
    std::string trackId;
    std::string title;
    std::string album;
    std::string artist;
    int duration;
    std::string iconUri;
};


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
    std::vector<Track> queryTracks(const std::string &pattern, int maxEntries = 10);
};

#endif
