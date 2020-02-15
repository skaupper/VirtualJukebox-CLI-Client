/*****************************************************************************/
/**
 * @file    deserializer.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of a generic deserializer function for the types used by the REST API.
 */
/*****************************************************************************/


#include "deserializer.h"


namespace api::v1::detail
{

void deserialize(const json &j, BaseTrack &track)
{
    track.trackId  = j.at("track_id");
    track.title    = j.at("title");
    track.duration = j.at("duration");
    track.iconUri  = j.at("icon_uri");

    if (j.find("album") != j.end()) {
        track.album  = j["album"];
    } else {
        track.album  = std::nullopt;
    }
    if (j.find("artist") != j.end()) {
        track.artist = j["artist"];
    } else {
        track.artist = std::nullopt;
    }
}

void deserialize(const json &j, QueueTrack &track)
{
    deserialize(j, (BaseTrack &)track);
    track.addedBy = j.at("added_by");
}

void deserialize(const json &j, NormalQueueTrack &track)
{
    deserialize(j, (QueueTrack &)track);
    track.votes       = j.at("votes");
    track.currentVote = j.at("current_vote");
}

void deserialize(const json &j, PlayingTrack &track)
{
    deserialize(j, (QueueTrack &)track);
    track.playing    = j.at("playing");
    track.playingFor = j.at("playing_for");
}

}
