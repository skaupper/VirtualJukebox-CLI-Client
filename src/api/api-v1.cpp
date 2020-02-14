#include "api-v1.h"

#include "json/json.hpp"
#include <connection.h>

#include <iostream>
#include <sstream>


using json = nlohmann::json;


static std::string getRequestUrl(const std::string &address, int port, const std::string &endpoint)
{
    static const std::string ENDPOINT_BASE_PATH = "/api/v1";

    std::stringstream urlStream;
    urlStream << "http://" << address << ":" << port << ENDPOINT_BASE_PATH << "/";
    urlStream << endpoint;
    return urlStream.str();
}


void APIv1::setServerAddress(const std::string &address, int port) noexcept
{
    mAddress = address;
    mPort = port;
}

std::string APIv1::getSessionId() const
{
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    return mSessionId;
}


bool APIv1::isAdmin() const noexcept
{
    return mIsAdmin;
}

bool APIv1::isSessionGenerated() const noexcept
{
    return mIsSessionGenerated;
}


void APIv1::generateSession(const std::string &nickname)
{
    static const std::string ENDPOINT = "generateSession";

    json requestBody = {
        { "nickname", nickname }
    };
    std::string url = getRequestUrl(mAddress, mPort, ENDPOINT);


    auto resp = RestClient::post(url, "application/json", requestBody.dump());
    if (resp.code != 200) {
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("JSON response has a invalid format", resp.body);
    }


    mSessionId = body["session_id"];
    mIsAdmin = false;
    mIsSessionGenerated = true;
}

void APIv1::generateAdminSession(const std::string &nickname, const std::string &adminPassword)
{
    (void) nickname;
    (void) adminPassword;
}
