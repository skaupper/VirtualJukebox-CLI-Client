/*****************************************************************************/
/**
 * @file    api-v1.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of REST API version 1
 */
/*****************************************************************************/

#include "api-v1.h"

#include "deserializer.h"
#include "http-status.h"

#include "exceptions/APIException.h"
#include "exceptions/InvalidFormatException.h"
#include "exceptions/NetworkException.h"

#include <httplib/httplib.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <map>
#include <sstream>


using json = nlohmann::json;
using namespace api::v1;


// static std::string to_string(PlayerAction action) {
//     switch (action) {
//     case PlayerAction::PLAY:
//         return "play";
//     case PlayerAction::PAUSE:
//         return "pause";
//     case PlayerAction::SKIP:
//         return "skip";
//     case PlayerAction::VOLUME_UP:
//         return "volume_up";
//     case PlayerAction::VOLUME_DOWN:
//         return "volume_down";

//     default:
//         throw APIException(APIExceptionCode::UNKNOWN_ENUM_VARIANT);
//     }
// }

static std::string to_string(QueueType queueType) {
    switch (queueType) {
    case QueueType::NORMAL:
        return "normal";
    case QueueType::ADMIN:
        return "admin";

    default:
        throw APIException(APIExceptionCode::UNKNOWN_ENUM_VARIANT);
    }
}

static std::string getRequestEndpoint(const std::string &endpoint) {
    constexpr auto ENDPOINT_BASE_PATH {"/api/v1"};
    return std::string(ENDPOINT_BASE_PATH) + "/" + endpoint;
}

static std::string getRequestEndpoint(const std::string &endpoint,
                                      const std::map<std::string, std::string> &parameters) {
    std::stringstream urlStream;
    urlStream << getRequestEndpoint(endpoint);

    bool firstParameter {true};

    std::for_each(std::cbegin(parameters), std::cend(parameters), [&](const auto &kv) {
        if (firstParameter) {
            urlStream << "?";
        } else {
            urlStream << "&";
        }
        urlStream << kv.first << "=" << kv.second;
        firstParameter = false;
    });

    return urlStream.str();
}


void APIv1::setServerAddress(const std::string &address, int port) noexcept {
    mAddress = address;
    mPort    = port;
}

std::string APIv1::getSessionId() const {
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    return mSessionId;
}


bool APIv1::isAdmin() const noexcept { return mIsAdmin; }

bool APIv1::isSessionGenerated() const noexcept { return mIsSessionGenerated; }


void APIv1::generateSession(const std::string &nickname) {
    constexpr auto ENDPOINT {"generateSession"};

    const json requestBody = {
        {"nickname", nickname}  //
    };


    httplib::Client client {mAddress, mPort};
    const auto resp {client.Post(getRequestEndpoint(ENDPOINT).c_str(), requestBody.dump(), "application/json")};

    if (!resp) {
        throw NetworkException(NetworkExceptionCode::FAILED_TO_CONNECT);
    }
    if (resp->status != static_cast<int>(HttpStatus::OK)) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp->status));
    }

    json body;
    try {
        body = json::parse(resp->body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp->body);
    }

    try {
        mSessionId = body.at("session_id");
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("Response misses field 'session_id'", body.dump());
    }
    mIsAdmin            = false;
    mIsSessionGenerated = true;
}

void APIv1::generateAdminSession(const std::string &nickname, const std::string &adminPassword) {
    constexpr auto ENDPOINT {"generateSession"};

    const json requestBody = {
        {"nickname", nickname},      //
        {"password", adminPassword}  //
    };

    httplib::Client client {mAddress, mPort};
    const auto resp {client.Post(getRequestEndpoint(ENDPOINT).c_str(), requestBody.dump(), "application/json")};

    if (!resp) {
        throw NetworkException(NetworkExceptionCode::FAILED_TO_CONNECT);
    }
    if (resp->status == static_cast<int>(HttpStatus::UNAUTHORIZED)) {
        throw APIException(APIExceptionCode::INVALID_PASSWORD);
    }
    if (resp->status != static_cast<int>(HttpStatus::OK)) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp->status));
    }

    json body;
    try {
        body = json::parse(resp->body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp->body);
    }


    try {
        mSessionId = body.at("session_id");
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("Response misses field 'session_id'", body.dump());
    }
    mIsAdmin            = true;
    mIsSessionGenerated = true;
}


std::vector<BaseTrack> APIv1::queryTracks(const std::string &pattern, int maxEntries) const {
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    constexpr auto ENDPOINT {"queryTracks"};

    const std::map<std::string, std::string> parameters {
        {"pattern", pattern},                        //
        {"max_entries", std::to_string(maxEntries)}  //
    };
    const std::string url {getRequestEndpoint(ENDPOINT, parameters)};


    httplib::Client client {mAddress, mPort};
    const auto resp {client.Get(url.c_str())};

    if (!resp) {
        throw NetworkException(NetworkExceptionCode::FAILED_TO_CONNECT);
    }
    if (resp->status != static_cast<int>(HttpStatus::OK)) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp->status));
    }

    json body;
    try {
        body = json::parse(resp->body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp->body);
    }

    try {
        return deserialize<std::vector<BaseTrack>>(body.at("tracks"));
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("An expected field could not be found in JSON object.", resp->body);
    }
}


Queue APIv1::getCurrentQueues() const {
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    constexpr auto ENDPOINT {"getCurrentQueues"};

    const std::map<std::string, std::string> parameters {
        {"session_id", mSessionId}  //
    };
    const std::string url {getRequestEndpoint(ENDPOINT, parameters)};


    httplib::Client client {mAddress, mPort};
    const auto resp {client.Get(url.c_str())};

    if (!resp) {
        throw NetworkException(NetworkExceptionCode::FAILED_TO_CONNECT);
    }
    if (resp->status != static_cast<int>(HttpStatus::OK)) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp->status));
    }

    json body;
    try {
        body = json::parse(resp->body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp->body);
    }


    try {
        Queue queue;
        queue.currentlyPlaying = std::nullopt;
        if (body.find("currently_playing") != body.end() && !body["currently_playing"].empty()) {
            queue.currentlyPlaying = deserialize<PlayingTrack>(body["currently_playing"]);
        }

        queue.normalQueue = deserialize<std::vector<NormalQueueTrack>>(body.at("normal_queue"));
        queue.adminQueue  = deserialize<std::vector<QueueTrack>>(body.at("admin_queue"));
        return queue;
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("An expected field could not be found in JSON object.", resp->body);
    }
}

void APIv1::addTrack(const BaseTrack &track, QueueType queueType) const {
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }
    if (queueType == QueueType::ADMIN && !isAdmin()) {
        throw APIException(APIExceptionCode::ADMIN_REQUIRED);
    }

    constexpr auto ENDPOINT {"addTrackToQueue"};

    const json requestBody {
        {"session_id", mSessionId},           //
        {"track_id", track.trackId},          //
        {"queue_type", to_string(queueType)}  //
    };
    const std::string url {getRequestEndpoint(ENDPOINT)};


    httplib::Client client {mAddress, mPort};
    const auto resp {client.Post(url.c_str(), requestBody.dump(), "application/json")};

    // TODO: this endpoint may fail with other errors too
    if (!resp) {
        throw NetworkException(NetworkExceptionCode::FAILED_TO_CONNECT);
    }
    if (resp->status != static_cast<int>(HttpStatus::OK)) {
        std::cerr << resp->body << std::endl;
        throw NetworkException(static_cast<NetworkExceptionCode>(resp->status));
    }

    json body;
    try {
        body = json::parse(resp->body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp->body);
    }

    // TODO: parse error message (if any)
}

// void APIv1::voteTrack(const BaseTrack &track, Vote vote) const
// {
//     if (!isSessionGenerated()) {
//         throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
//     }

//     static const std::string ENDPOINT = "voteTrack";

//     const json requestBody = {
//         { "session_id", mSessionId },
//         { "track_id", track.trackId },
//         { "vote", static_cast<int>(vote) }
//     };
//     const std::string url = getRequestEndpoint(ENDPOINT);


//     const auto resp = RestClient::put(url, "application/json", requestBody.dump());
//     // TODO: this endpoint may fail with other errors too
//     if (resp->status != static_cast<int>(HttpStatus::OK)) {
//         throw NetworkException(static_cast<NetworkExceptionCode>(resp->status));
//     }

//     json body;
//     try {
//         body = json::parse(resp->body);
//     } catch (...) {
//         throw InvalidFormatException("Response body is no valid JSON", resp->body);
//     }

//     // TODO: parse error message (if any)
// }


// void APIv1::controlPlayer(PlayerAction action) const
// {
//     if (!isSessionGenerated()) {
//         throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
//     }
//     if (!isAdmin()) {
//         throw APIException(APIExceptionCode::ADMIN_REQUIRED);
//     }

//     static const std::string ENDPOINT = "controlPlayer";

//     const json requestBody = {
//         { "session_id", mSessionId },
//         { "player_action", to_string(action) }
//     };
//     const std::string url = getRequestEndpoint(ENDPOINT);


//     const auto resp = RestClient::put(url, "application/json", requestBody.dump());
//     // TODO: this endpoint may fail with other errors too
//     if (resp->status != static_cast<int>(HttpStatus::OK)) {
//         throw NetworkException(static_cast<NetworkExceptionCode>(resp->status));
//     }

//     json body;
//     try {
//         body = json::parse(resp->body);
//     } catch (...) {
//         throw InvalidFormatException("Response body is no valid JSON", resp->body);
//     }

//     // TODO: parse error message (if any)
// }

// void APIv1::moveTrack(const BaseTrack &track, QueueType queueType) const
// {
//     if (!isSessionGenerated()) {
//         throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
//     }
//     if (!isAdmin()) {
//         throw APIException(APIExceptionCode::ADMIN_REQUIRED);
//     }

//     static const std::string ENDPOINT = "moveTrack";

//     const json requestBody = {
//         { "session_id", mSessionId },
//         { "track_id", track.trackId },
//         { "queue_type", to_string(queueType) }
//     };
//     const std::string url = getRequestEndpoint(ENDPOINT);


//     const auto resp = RestClient::put(url, "application/json", requestBody.dump());
//     // TODO: this endpoint may fail with other errors too
//     if (resp->status != static_cast<int>(HttpStatus::OK)) {
//         throw NetworkException(static_cast<NetworkExceptionCode>(resp->status));
//     }

//     json body;
//     try {
//         body = json::parse(resp->body);
//     } catch (...) {
//         throw InvalidFormatException("Response body is no valid JSON", resp->body);
//     }

//     // TODO: parse error message (if any)
// }

// void APIv1::removeTrack(const BaseTrack &) const
// {
//     // This endpoint uses a request body for the DELETE endpoint
//     // instead of query parameters, which is not supported by restclient-cpp
//     // TODO: implement as soon as the server uses query parameters instead
//     throw APIException(APIExceptionCode::NOT_IMPLEMENTED);
// }
