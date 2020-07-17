/*****************************************************************************/
/**
 * @file    api-v1.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of REST API version 1
 */
/*****************************************************************************/

#ifndef API_V1_H
#define API_V1_H

#include "api-types.h"

#include <httplib/httplib.h>
#include <nlohmann/json.hpp>


namespace api::v1 {

    class APIv1 {
        std::string mAddress;
        unsigned int mPort;
        httplib::Client mClient;

        std::string mSessionId;
        bool mIsAdmin;
        bool mIsSessionGenerated = false;

    private:
        nlohmann::json doGetRequest(const char *const url);
        nlohmann::json doGetRequest(const std::string &url);

        nlohmann::json doPostRequest(const char *const url, const nlohmann::json &requestBody);
        nlohmann::json doPostRequest(const std::string &url, const nlohmann::json &requestBody);

        nlohmann::json doPutRequest(const char *const url, const nlohmann::json &requestBody);
        nlohmann::json doPutRequest(const std::string &url, const nlohmann::json &requestBody);

        nlohmann::json doDeleteRequest(const char *const url, const nlohmann::json &requestBody);
        nlohmann::json doDeleteRequest(const std::string &url, const nlohmann::json &requestBody);


    public:
        APIv1(const std::string &address, const unsigned int port) noexcept;

        std::string getSessionId() const;

        bool isAdmin() const noexcept;
        bool isSessionGenerated() const noexcept;


        //
        // API methods
        //

        void generateSession(const std::optional<std::string> &nickname);
        void generateAdminSession(const std::string &adminPassword, const std::optional<std::string> &nickname);
        std::vector<BaseTrack> queryTracks(const std::string &pattern, int maxEntries = 10);
        Queues getCurrentQueues();
        void addTrack(const BaseTrack &, QueueType = QueueType::NORMAL);
        void voteTrack(const BaseTrack &, Vote vote);
        void controlPlayer(PlayerAction action);
        void moveTrack(const BaseTrack &, QueueType);
        void removeTrack(const BaseTrack &);
    };

}  // namespace api::v1

#endif
