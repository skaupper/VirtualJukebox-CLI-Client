#include "api-v1.h"

#include "json/json.hpp"
#include <connection.h>

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


void APIv1::setServerAddress(const std::string &address, int port)
{
    this->address = address;
    this->port = port;
}


void APIv1::generateSession(const std::string &nickname)
{
    static const std::string ENDPOINT = "generateSession";

    json requestBody = {
        { "nickname", nickname }
    };
    std::string url = getRequestUrl(address, port, ENDPOINT);

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

    this->sessionId = body["session_id"];
    this->isAdmin = false;
    this->isSessionGenerated = true;
}

void APIv1::generateAdminSession(const std::string &nickname, const std::string &adminPassword)
{
    (void) nickname;
    (void) adminPassword;
}
