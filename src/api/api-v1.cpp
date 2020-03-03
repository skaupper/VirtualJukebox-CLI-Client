/*****************************************************************************/
/**
 * @file    api-v1.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of REST API version 1
 */
/*****************************************************************************/

#include "api-v1.h"

#include "deserializer.h"

#include "exceptions/APIException.h"
#include "exceptions/NetworkException.h"
#include "exceptions/InvalidFormatException.h"

#include "json/json.hpp"
#include <connection.h>

#include <iostream>
#include <sstream>
#include <map>


using json = nlohmann::json;
using namespace api::v1;


static std::string to_string(PlayerAction action)
{
    switch (action)
    {
    case PlayerAction::PLAY:        return "play";
    case PlayerAction::PAUSE:       return "pause";
    case PlayerAction::SKIP:        return "skip";
    case PlayerAction::VOLUME_UP:   return "volume_up";
    case PlayerAction::VOLUME_DOWN: return "volume_down";

    default: throw APIExceptionCode(APIExceptionCode::UNKNOWN_ENUM_VARIANT);
    }
}

static std::string to_string(QueueType queueType)
{
    switch (queueType)
    {
    case QueueType::NORMAL: return "normal";
    case QueueType::ADMIN:  return "admin";

    default: throw APIExceptionCode(APIExceptionCode::UNKNOWN_ENUM_VARIANT);
    }
}

static std::string getRequestEndpoint(const std::string &endpoint, const std::map<std::string, std::string> &parameters = {})
{
    static const std::string ENDPOINT_BASE_PATH = "/api/v1";

    std::stringstream endpointStream;
    endpointStream << ENDPOINT_BASE_PATH << "/" << endpoint;

    bool firstParameter = true;
    for (const auto &kv: parameters) {
        if (firstParameter) {
            endpointStream << "?";
        } else {
            endpointStream << "&";
        }
        endpointStream << kv.first << "=" << kv.second;
        firstParameter = false;
    }

    return endpointStream.str();
}

static std::string getRequestUrl(const std::string &address, int port)
{
    std::stringstream urlStream;
    urlStream << "http://" << address << ":" << port;
    return urlStream.str();
}


static RestClient::Connection createDefaultConnection(const std::string &url)
{
    RestClient::Connection conn(url);
    conn.SetTimeout(2);
    return conn;
}

static RestClient::Connection createDefaultConnection(const std::string &url, const std::string &contentType)
{
    auto conn = createDefaultConnection(url);
    conn.AppendHeader("Content-Type", contentType);
    return conn;
}


void api::v1::Initialize()
{
    RestClient::init();
}

void api::v1::Deinitialize()
{
    RestClient::disable();
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
    static const std::string ENDPOINT = "/generateSession";

    const json requestBody = {
        { "nickname", nickname }
    };
    const std::string url       = getRequestUrl(mAddress, mPort);
    const std::string endpoint  = getRequestEndpoint(ENDPOINT);


    const auto resp = createDefaultConnection(url, "application/json").post(endpoint, requestBody.dump());
    if (resp.code != 200) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }

    try {
        mSessionId      = body.at("session_id");
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("Response misses field 'session_id'", body.dump());
    }
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
    const std::string url       = getRequestUrl(mAddress, mPort);
    const std::string endpoint  = getRequestEndpoint(ENDPOINT);

    const auto resp = createDefaultConnection(url, "application/json").post(endpoint, requestBody.dump());
    if (resp.code == 401) {
        throw APIException(APIExceptionCode::INVALID_PASSWORD);
    } else if (resp.code != 200) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }


    try {
        mSessionId          = body.at("session_id");
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("Response misses field 'session_id'", body.dump());
    }
    mIsAdmin            = true;
    mIsSessionGenerated = true;
}


std::vector<BaseTrack> APIv1::queryTracks(const std::string &pattern, int maxEntries) const
{
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    static const std::string ENDPOINT = "queryTracks";

    const auto parameters = std::map<std::string, std::string> {
        { "pattern", pattern },
        { "max_entries", std::to_string(maxEntries) }
    };
    const std::string url       = getRequestUrl(mAddress, mPort);
    const std::string endpoint  = getRequestEndpoint(ENDPOINT, parameters);

    const auto resp = createDefaultConnection(url).get(endpoint);
    if (resp.code != 200) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }

    std::vector<BaseTrack> tracks;
    try {
        tracks = deserialize<std::vector<BaseTrack>>(body.at("tracks"));
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("An expected field could not be found in JSON object.", resp.body);
    }

    return tracks;
}


Queue APIv1::getCurrentQueues() const
{
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    static const std::string ENDPOINT = "getCurrentQueues";

    const auto parameters = std::map<std::string, std::string> {
        { "session_id", mSessionId }
    };
    const std::string url       = getRequestUrl(mAddress, mPort);
    const std::string endpoint  = getRequestEndpoint(ENDPOINT, parameters);

    const auto resp = createDefaultConnection(url).get(endpoint);
    if (resp.code != 200) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }


    Queue queue;
    try {
        queue.currentlyPlaying = std::nullopt;
        if (body.find("currently_playing") != body.end() && !body["currently_playing"].empty()) {
            queue.currentlyPlaying = deserialize<PlayingTrack>(body["currently_playing"]);
        }

        queue.normalQueue = deserialize<std::vector<NormalQueueTrack>>(body.at("normal_queue"));
        queue.adminQueue  = deserialize<std::vector<QueueTrack>>(body.at("admin_queue"));
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("An expected field could not be found in JSON object.", resp.body);
    }

    return queue;
}

void APIv1::addTrack(const BaseTrack &track, QueueType queueType) const
{
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }
    if (queueType == QueueType::ADMIN && !isAdmin()) {
        throw APIException(APIExceptionCode::ADMIN_REQUIRED);
    }

    static const std::string ENDPOINT = "addTrackToQueue";

    const json requestBody = {
        { "session_id", mSessionId },
        { "track_id", track.trackId },
        { "queue_type", to_string(queueType) }
    };
    const std::string url       = getRequestUrl(mAddress, mPort);
    const std::string endpoint  = getRequestEndpoint(ENDPOINT);

    const auto resp = createDefaultConnection(url, "application/json").post(endpoint, requestBody.dump());
    // TODO: this endpoint may fail with other errors too
    if (resp.code != 200) {
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }

    // TODO: parse error message (if any)
}

void APIv1::voteTrack(const BaseTrack &track, Vote vote) const
{
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    static const std::string ENDPOINT = "voteTrack";

    const json requestBody = {
        { "session_id", mSessionId },
        { "track_id", track.trackId },
        { "vote", static_cast<int>(vote) }
    };
    const std::string url       = getRequestUrl(mAddress, mPort);
    const std::string endpoint  = getRequestEndpoint(ENDPOINT);

    const auto resp = createDefaultConnection(url, "application/json").put(endpoint, requestBody.dump());
    // TODO: this endpoint may fail with other errors too
    if (resp.code != 200) {
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }

    // TODO: parse error message (if any)
}


void APIv1::controlPlayer(PlayerAction action) const
{
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }
    if (!isAdmin()) {
        throw APIException(APIExceptionCode::ADMIN_REQUIRED);
    }

    static const std::string ENDPOINT = "controlPlayer";

    const json requestBody = {
        { "session_id", mSessionId },
        { "player_action", to_string(action) }
    };
    const std::string url       = getRequestUrl(mAddress, mPort);
    const std::string endpoint  = getRequestEndpoint(ENDPOINT);

    const auto resp = createDefaultConnection(url, "application/json").put(endpoint, requestBody.dump());
    // TODO: this endpoint may fail with other errors too
    if (resp.code != 200) {
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }

    // TODO: parse error message (if any)
}

void APIv1::moveTrack(const BaseTrack &track, QueueType queueType) const
{
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }
    if (!isAdmin()) {
        throw APIException(APIExceptionCode::ADMIN_REQUIRED);
    }

    static const std::string ENDPOINT = "moveTrack";

    const json requestBody = {
        { "session_id", mSessionId },
        { "track_id", track.trackId },
        { "queue_type", to_string(queueType) }
    };
    const std::string url       = getRequestUrl(mAddress, mPort);
    const std::string endpoint  = getRequestEndpoint(ENDPOINT);

    const auto resp = createDefaultConnection(url, "application/json").put(endpoint, requestBody.dump());
    // TODO: this endpoint may fail with other errors too
    if (resp.code != 200) {
        throw NetworkException(static_cast<NetworkExceptionCode>(resp.code));
    }

    json body;
    try {
        body = json::parse(resp.body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp.body);
    }

    // TODO: parse error message (if any)
}

void APIv1::removeTrack(const BaseTrack &) const
{
    // This endpoint uses a request body for the DELETE endpoint
    // instead of query parameters, which is not supported by restclient-cpp
    // TODO: implement as soon as the server uses query parameters instead
    throw APIException(APIExceptionCode::NOT_IMPLEMENTED);
}
