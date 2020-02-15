/*****************************************************************************/
/**
 * @file    api-types.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of types used by the REST API version 1
 */
/*****************************************************************************/

#ifndef API_TYPES_H
#define API_TYPES_H

#include <string>
#include <vector>
#include <optional>


namespace api::v1
{

struct BaseTrack
{
    std::string trackId;
    std::string title;
    std::optional<std::string> album;
    std::optional<std::string> artist;
    int duration;
    std::string iconUri;
};

struct QueueTrack : public BaseTrack
{
    std::string addedBy;
};

struct NormalQueueTrack : public QueueTrack
{
    int votes;
    int currentVote;
};

struct PlayingTrack : public QueueTrack
{
    bool playing;
    int playingFor;
};


struct Queue
{
    std::optional<PlayingTrack> currentlyPlaying;
    std::vector<NormalQueueTrack> normalQueue;
    std::vector<QueueTrack> adminQueue;
};

}

#endif
