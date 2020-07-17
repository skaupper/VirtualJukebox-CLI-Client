/*****************************************************************************/
/**
 * @file    Api.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of REST API version 1
 */
/*****************************************************************************/

#include "Api.h"

#include "deserializer.h"
#include "utils/http-status.h"
#include "utils/utils.h"

#include "exceptions/APIException.h"
#include "exceptions/InvalidFormatException.h"
#include "exceptions/NetworkException.h"

#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <map>
#include <sstream>


using json = nlohmann::json;
using namespace api::v1;


//
// Helper functions
//


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


//
// Constructors and Getters
//

Api::Api(const std::string &address, const unsigned int port) noexcept
    : mAddress(address), mPort(port), mClient(address, int(port)) {}

std::string Api::getSessionId() const {
    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    return mSessionId;
}


bool Api::isAdmin() const noexcept { return mIsAdmin; }

bool Api::isSessionGenerated() const noexcept { return mIsSessionGenerated; }


//
// Request helpers
//

static void verifyResponse(const std::shared_ptr<httplib::Response> &resp) {
    if (!resp) {
        throw NetworkException(NetworkExceptionCode::FAILED_TO_CONNECT, "The response pointer was equal to nullptr.");
    }

    if (resp->status == static_cast<int>(sk::HttpStatus::UNAUTHORIZED)) {
        throw APIException(APIExceptionCode::INVALID_PASSWORD);
    }


    if (resp->status != static_cast<int>(sk::HttpStatus::OK)) {
        // TODO: parse error message (if any)
        throw NetworkException(static_cast<NetworkExceptionCode>(resp->status), resp->body);
    }
}

static json extractJsonBody(const std::shared_ptr<httplib::Response> &resp) {
    try {
        return json::parse(resp->body);
    } catch (...) {
        throw InvalidFormatException("Response body is no valid JSON", resp->body);
    }
}


json Api::doGetRequest(const char *const url) {
    spdlog::debug("Api::doGetRequest: {}", url);

    const auto resp {mClient.Get(url)};
    verifyResponse(resp);
    return extractJsonBody(resp);
}

json Api::doPostRequest(const char *const url, const json &requestBody) {
    spdlog::debug("Api::doPostRequest: {}, {}", url, requestBody.dump());
    const auto resp {mClient.Post(url, requestBody.dump(), "application/json")};

    verifyResponse(resp);
    return extractJsonBody(resp);
}

json Api::doPutRequest(const char *const url, const json &requestBody) {
    spdlog::debug("Api::doPutRequest: {}, {}", url, requestBody.dump());
    const auto resp {mClient.Put(url, requestBody.dump(), "application/json")};

    verifyResponse(resp);
    return extractJsonBody(resp);
}

json Api::doDeleteRequest(const char *const url, const json &requestBody) {
    spdlog::debug("Api::doDeleteRequest: {}, {}", url, requestBody.dump());
    const auto resp {mClient.Delete(url, requestBody.dump(), "application/json")};

    verifyResponse(resp);
    return extractJsonBody(resp);
}


json Api::doGetRequest(const std::string &url) { return doGetRequest(url.c_str()); }
json Api::doPostRequest(const std::string &url, const json &requestBody) {
    return doPostRequest(url.c_str(), requestBody);
}
json Api::doPutRequest(const std::string &url, const json &requestBody) {
    return doPutRequest(url.c_str(), requestBody);
}
json Api::doDeleteRequest(const std::string &url, const json &requestBody) {
    return doDeleteRequest(url.c_str(), requestBody);
}


//
// Actual endpoint implementations
//

void Api::generateSession(const std::optional<std::string> &nickname) {
    spdlog::debug("Api::generateSession: {}", nickname);

    constexpr auto ENDPOINT {"generateSession"};

    json requestBody;
    if (nickname) {
        requestBody["nickname"] = nickname.value();
    }

    const auto body = doPostRequest(getRequestEndpoint(ENDPOINT), requestBody);

    try {
        mSessionId = body.at("session_id");
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("Response misses field 'session_id'", body.dump());
    } catch (const json::type_error &) {
        throw InvalidFormatException("Received JSON object is of wrong type", body.dump());
    }

    mIsAdmin            = false;
    mIsSessionGenerated = true;
}

void Api::generateAdminSession(const std::string &adminPassword, const std::optional<std::string> &nickname) {
    spdlog::debug("Api::generateAdminSession: {}, {}", nickname, adminPassword);

    constexpr auto ENDPOINT {"generateSession"};

    json requestBody {
        {"password", adminPassword}  //
    };
    if (nickname) {
        requestBody["nickname"] = nickname.value();
    }

    const auto body = doPostRequest(getRequestEndpoint(ENDPOINT), requestBody);


    try {
        mSessionId = body.at("session_id");
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("Response misses field 'session_id'", body.dump());
    } catch (const json::type_error &) {
        throw InvalidFormatException("Received JSON object is of wrong type", body.dump());
    }


    mIsAdmin            = true;
    mIsSessionGenerated = true;
}


std::vector<BaseTrack> Api::queryTracks(const std::string &pattern, const unsigned int maxEntries) {
    spdlog::debug("Api::queryTracks: {}, {}", pattern, maxEntries);

    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    constexpr auto ENDPOINT {"queryTracks"};

    const std::map<std::string, std::string> parameters {
        {"pattern", pattern},                        //
        {"max_entries", std::to_string(maxEntries)}  //
    };
    const auto body = doGetRequest(getRequestEndpoint(ENDPOINT, parameters));


    try {
        return deserialize<std::vector<BaseTrack>>(body.at("tracks"));
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("An expected field could not be found in JSON object.", body.dump());
    } catch (const json::type_error &) {
        throw InvalidFormatException("Received JSON object is of wrong type", body.dump());
    }
}


Queues Api::getCurrentQueues() {
    spdlog::debug("Api::getCurrentQueues");

    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    constexpr auto ENDPOINT {"getCurrentQueues"};

    const std::map<std::string, std::string> parameters {
        {"session_id", mSessionId}  //
    };
    const auto body = doGetRequest(getRequestEndpoint(ENDPOINT, parameters));


    try {
        return deserialize<Queues>(body);
    } catch (const json::out_of_range &) {
        throw InvalidFormatException("An expected field could not be found in JSON object.", body.dump());
    }
}

void Api::addTrack(const BaseTrack &track, const QueueType queueType) {
    spdlog::debug("Api::addTrack: {}, {}", track.trackId, to_string(queueType));

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

    const auto body = doPostRequest(getRequestEndpoint(ENDPOINT), requestBody);
}

void Api::voteTrack(const BaseTrack &track, const Vote vote) {
    spdlog::debug("Api::voteTrack: {}, {}", track.trackId, vote);

    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }

    constexpr auto ENDPOINT {"voteTrack"};

    const json requestBody {
        {"session_id", mSessionId},       //
        {"track_id", track.trackId},      //
        {"vote", static_cast<int>(vote)}  //
    };
    const auto body = doPutRequest(getRequestEndpoint(ENDPOINT), requestBody);
}


void Api::controlPlayer(const PlayerAction action) {
    spdlog::debug("Api::controlPlayer: {}", to_string(action));

    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }
    if (!isAdmin()) {
        throw APIException(APIExceptionCode::ADMIN_REQUIRED);
    }

    constexpr auto ENDPOINT {"controlPlayer"};

    const json requestBody {
        {"session_id", mSessionId},           //
        {"player_action", to_string(action)}  //
    };
    const auto body = doPutRequest(getRequestEndpoint(ENDPOINT), requestBody);
}

void Api::moveTrack(const BaseTrack &track, const QueueType queueType) {
    spdlog::debug("Api::moveTrack: {}, {}", track.trackId, to_string(queueType));

    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }
    if (!isAdmin()) {
        throw APIException(APIExceptionCode::ADMIN_REQUIRED);
    }

    constexpr auto ENDPOINT {"moveTrack"};

    const json requestBody {
        {"session_id", mSessionId},           //
        {"track_id", track.trackId},          //
        {"queue_type", to_string(queueType)}  //
    };
    const auto body = doPutRequest(getRequestEndpoint(ENDPOINT), requestBody);
}

void Api::removeTrack(const BaseTrack &track) {
    spdlog::debug("Api::removeTrack: {}", track.trackId);

    if (!isSessionGenerated()) {
        throw APIException(APIExceptionCode::NO_SESSION_GENERATED);
    }
    if (!isAdmin()) {
        throw APIException(APIExceptionCode::ADMIN_REQUIRED);
    }

    constexpr auto ENDPOINT {"removeTrack"};

    const json requestBody {
        {"session_id", mSessionId},  //
        {"track_id", track.trackId}  //
    };
    const auto body = doDeleteRequest(getRequestEndpoint(ENDPOINT), requestBody);
}
