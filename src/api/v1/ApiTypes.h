/*****************************************************************************/
/**
 * @file    APITypes.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of types used by the REST API version 1
 */
/*****************************************************************************/

#ifndef API_V1_TYPES_H
#define API_V1_TYPES_H

#include <optional>
#include <string>
#include <vector>

#include "exceptions/APIException.h"


namespace api::v1 {

    struct BaseTrack {
        std::string trackId;
        std::string title;
        std::optional<std::string> album;
        std::optional<std::string> artist;
        int duration;
        std::string iconUri;
    };

    struct QueueTrack : public BaseTrack {
        std::string addedBy;
    };

    struct NormalQueueTrack : public QueueTrack {
        int votes;
        int currentVote;
    };

    struct PlayingTrack : public QueueTrack {
        bool playing;
        int playingFor;
    };


    struct Queues {
        std::optional<PlayingTrack> currentlyPlaying;
        std::vector<NormalQueueTrack> normalQueue;
        std::vector<QueueTrack> adminQueue;
    };

    enum class QueueType { NORMAL = 0, ADMIN = 1 };


    enum class Vote { DOWN_VOTE = 0, UP_VOTE = 1 };

    enum class PlayerAction {
        PLAY,
        PAUSE,
        SKIP,
        VOLUME_UP,
        VOLUME_DOWN,
    };


    //
    // Stringification helpers
    //

    inline std::string to_string(PlayerAction action) {
        switch (action) {
        case PlayerAction::PLAY:
            return "play";
        case PlayerAction::PAUSE:
            return "pause";
        case PlayerAction::SKIP:
            return "skip";
        case PlayerAction::VOLUME_UP:
            return "volume_up";
        case PlayerAction::VOLUME_DOWN:
            return "volume_down";

        default:
            throw APIException(APIExceptionCode::UNKNOWN_ENUM_VARIANT);
        }
    }

    inline std::string to_string(QueueType queueType) {
        switch (queueType) {
        case QueueType::NORMAL:
            return "normal";
        case QueueType::ADMIN:
            return "admin";

        default:
            throw APIException(APIExceptionCode::UNKNOWN_ENUM_VARIANT);
        }
    }


    template<typename T>
    inline T from_string(const std::string &str);

    template<>
    inline QueueType from_string(const std::string &str) {
        if (str == "normal") {
            return QueueType::NORMAL;
        } else if (str == "admin") {
            return QueueType::ADMIN;
        } else {
            throw APIException(APIExceptionCode::UNKNOWN_ENUM_VARIANT);
        }
    }

    template<>
    inline PlayerAction from_string(const std::string &str) {
        if (str == "play") {
            return PlayerAction::PLAY;
        } else if (str == "pause") {
            return PlayerAction::PAUSE;
        } else if (str == "skip") {
            return PlayerAction::SKIP;
        } else if (str == "volume_up") {
            return PlayerAction::VOLUME_UP;
        } else if (str == "volume_down") {
            return PlayerAction::VOLUME_DOWN;
        } else {
            throw APIException(APIExceptionCode::UNKNOWN_ENUM_VARIANT);
        }
    }

}  // namespace api::v1

#endif
