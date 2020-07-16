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

namespace api::v1 {

    class APIv1 {
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


        //
        // API methods
        //

        void generateSession(const std::string &nickname);
        void generateAdminSession(const std::string &nickname, const std::string &adminPassword);
        std::vector<BaseTrack> queryTracks(const std::string &pattern, int maxEntries = 10) const;
        Queue getCurrentQueues() const;
        void addTrack(const BaseTrack &, QueueType = QueueType::NORMAL) const;
        void voteTrack(const BaseTrack &, Vote vote) const;
        void controlPlayer(PlayerAction action) const;
        void moveTrack(const BaseTrack &, QueueType) const;
        void removeTrack(const BaseTrack &) const;
    };

}  // namespace api::v1

#endif
