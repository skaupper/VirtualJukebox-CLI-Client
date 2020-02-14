#include "api-v1.h"

#include "exceptions/APIException.h"
#include "exceptions/NetworkException.h"
#include "exceptions/InvalidFormatException.h"

#include "json/json.hpp"
#include <connection.h>

#include <iostream>
#include <sstream>
#include <map>


using json = nlohmann::json;


static std::string getRequestUrl(const std::string &address, int port, const std::string &endpoint, const std::map<std::string, std::string> &parameters = {})
{
    static const std::string ENDPOINT_BASE_PATH = "/api/v1";

    std::stringstream urlStream;
    urlStream << "http://" << address << ":" << port << ENDPOINT_BASE_PATH << "/";
    urlStream << endpoint;


    bool firstParameter = true;
    for (const auto &kv: parameters) {
        if (firstParameter) {
            urlStream << "?";
        } else {
            urlStream << "&";
        }
        urlStream << kv.first << "=" << kv.second;
        firstParameter = false;
    }

    return urlStream.str();
}


void APIv1::setServerAddress(const std::string &address, int port) noexcept
{
    mAddress = address;
    mPort    = port;
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

    const json requestBody = {
        { "nickname", nickname }
    };
    const std::string url = getRequestUrl(mAddress, mPort, ENDPOINT);


    const auto resp = RestClient::post(url, "application/json", requestBody.dump());
    if (resp.code != 200) {
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }


    mSessionId          = body["session_id"];
    mIsAdmin            = false;
    mIsSessionGenerated = true;
}

void APIv1::generateAdminSession(const std::string &nickname, const std::string &adminPassword)
{
    static const std::string ENDPOINT = "generateSession";

    const json requestBody = {
        { "nickname", nickname },
        { "password", adminPassword }
    };
    const std::string url = getRequestUrl(mAddress, mPort, ENDPOINT);


    const auto resp = RestClient::post(url, "application/json", requestBody.dump());
    if (resp.code == 401) {
        throw APIException(APIExceptionCode::INVALID_PASSWORD);
    } else if (resp.code != 200) {
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }


    mSessionId          = body["session_id"];
    mIsAdmin            = true;
    mIsSessionGenerated = true;
}


std::vector<Track> APIv1::queryTracks(const std::string &pattern, int maxEntries)
{
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    static const std::string ENDPOINT = "queryTracks";

    const auto parameters = std::map<std::string, std::string> {
        { "pattern", pattern },
        { "max_entries", std::to_string(maxEntries) }
    };
    const std::string url = getRequestUrl(mAddress, mPort, ENDPOINT, parameters);


    const auto resp = RestClient::get(url);
    if (resp.code != 200) {
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }

    std::vector<Track> tracks;
    for (const auto &jsonTrack: body["tracks"]) {
        // TODO: check for non-existent fields
        Track track;
        track.trackId  = jsonTrack["track_id"];
        track.title    = jsonTrack["title"];
        track.album    = jsonTrack["album"];
        track.artist   = jsonTrack["artist"];
        track.duration = jsonTrack["duration"];
        track.iconUri  = jsonTrack["icon_uri"];
        tracks.emplace_back(track);
    }

    return tracks;
}
