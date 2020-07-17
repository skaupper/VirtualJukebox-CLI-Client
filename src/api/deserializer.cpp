/*****************************************************************************/
/**
 * @file    deserializer.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of a generic deserializer function for the types used by the REST API.
 */
/*****************************************************************************/


#include "deserializer.h"


namespace api::v1::detail {

    void deserialize(const json &j, BaseTrack &track) {
        track.trackId  = j.at("track_id");
        track.title    = j.at("title");
        track.duration = j.at("duration");
        track.iconUri  = j.at("icon_uri");

        if (j.find("album") != j.end()) {
            track.album = j["album"];
        } else {
            track.album = std::nullopt;
        }
        if (j.find("artist") != j.end()) {
            track.artist = j["artist"];
        } else {
            track.artist = std::nullopt;
        }
    }

    void deserialize(const json &j, QueueTrack &track) {
        deserialize(j, static_cast<BaseTrack &>(track));
        track.addedBy = j.at("added_by");
    }

    void deserialize(const json &j, NormalQueueTrack &track) {
        deserialize(j, static_cast<QueueTrack &>(track));
        track.votes       = j.at("votes");
        track.currentVote = j.at("current_vote");
    }

    void deserialize(const json &j, PlayingTrack &track) {
        deserialize(j, static_cast<QueueTrack &>(track));
        track.playing    = j.at("playing");
        track.playingFor = j.at("playing_for");
    }


    void deserialize(const json &j, Queues &queue) {
        queue.currentlyPlaying = std::nullopt;
        if (j.find("currently_playing") != j.end() && !j["currently_playing"].empty()) {
            queue.currentlyPlaying = api::v1::deserialize<PlayingTrack>(static_cast<json>(j["currently_playing"]));
        }

        queue.normalQueue = api::v1::deserialize<std::vector<NormalQueueTrack>>(j.at("normal_queue"));
        queue.adminQueue  = api::v1::deserialize<std::vector<QueueTrack>>(j.at("admin_queue"));
    }

}  // namespace api::v1::detail
