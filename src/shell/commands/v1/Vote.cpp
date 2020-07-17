#include "ApiCommands.h"

#include "utils/utils.h"

#include "api/v1/Api.h"
#include "exceptions/ShellException.h"


using namespace api::v1;


//
// Helper functions
//

static std::optional<NormalQueueTrack> voteForTrack(std::ostream &out, std::istream &in,
                                                    const std::vector<NormalQueueTrack> &tracks) {
    bool printed {false};
    int trackRank {1};
    std::for_each(std::cbegin(tracks), std::cend(tracks), [&](const auto &track) {
        if (track.currentVote == 0) {
            out << fmt::format("{}: {} - {}", trackRank, track.title, track.artist) << std::endl;
            printed = true;
        }
        ++trackRank;
    });

    if (!printed) {
        out << "No track to vote for." << std::endl;
        return std::nullopt;
    }

    // Ask the user to select a track
    out << "Which track do you want to vote for? ";

    std::string line;
    if (!std::getline(in, line).good()) {
        return std::nullopt;
    }

    if (line.empty()) {
        return std::nullopt;
    }


    // Check if the input is a number and in range
    const auto optTrackNumber {sk::to_number<unsigned int>(line)};
    if (!optTrackNumber) {
        out << "Input was not a valid number!" << std::endl;
        return std::nullopt;
    }

    const auto trackNumber {optTrackNumber.value()};
    if (trackNumber == 0 || trackNumber > std::size(tracks)) {
        out << "Invalid number!" << std::endl;
        return std::nullopt;
    }

    const auto track {tracks[trackNumber - 1]};
    if (track.currentVote != 0) {
        out << "A vote for this track is already placed!" << std::endl;
        return std::nullopt;
    }

    return tracks[trackNumber - 1];
}

static std::optional<NormalQueueTrack> revokeVoteForTrack(std::ostream &out, std::istream &in,
                                                          const std::vector<NormalQueueTrack> &tracks) {
    bool printed {false};
    int trackRank {1};
    std::for_each(std::cbegin(tracks), std::cend(tracks), [&](const auto &track) {
        if (track.currentVote != 0) {
            out << fmt::format("{}: {} - {}", trackRank, track.title, track.artist) << std::endl;
            printed = true;
        }
        ++trackRank;
    });

    if (!printed) {
        out << "No votes to be revoked." << std::endl;
        return std::nullopt;
    }

    // Ask the user to select a track
    out << "Which vote do you want to revoke? ";

    std::string line;
    if (!std::getline(in, line).good()) {
        return std::nullopt;
    }

    if (line.empty()) {
        return std::nullopt;
    }


    // Check if the input is a number and in range
    const auto optTrackNumber {sk::to_number<unsigned int>(line)};
    if (!optTrackNumber) {
        out << "Input was not a valid number!" << std::endl;
        return std::nullopt;
    }

    const auto trackNumber {optTrackNumber.value()};
    if (trackNumber == 0 || trackNumber > std::size(tracks)) {
        out << "Invalid number!" << std::endl;
        return std::nullopt;
    }

    const auto track {tracks[trackNumber - 1]};
    if (track.currentVote == 0) {
        out << "No vote found for this track!" << std::endl;
        return std::nullopt;
    }

    return tracks[trackNumber - 1];
}


//
// Actual command
//

namespace commands::v1 {

    void Vote::doExecute(const std::vector<std::string> &args) {

        if (std::size(args) != 1) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
        }

        auto api = api::v1::Api::getInstance();

        const std::string dir {args[0]};
        if (dir != "up" && dir != "revoke") {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_VALUE);
        }

        const auto queues {api->getCurrentQueues()};
        if (dir == "up") {
            const auto optTrack {voteForTrack(getOut(), getIn(), queues.normalQueue)};
            if (!optTrack) {
                return;
            }
            const auto track {optTrack.value()};

            api->voteTrack(track, api::v1::Vote::UP_VOTE);
            getOut() << fmt::format("Vote placed for track '{}' by '{}'.", track.title, track.artist) << std::endl;
        } else if (dir == "revoke") {
            const auto optTrack {revokeVoteForTrack(getOut(), getIn(), queues.normalQueue)};
            if (!optTrack) {
                return;
            }
            const auto track {optTrack.value()};

            api->voteTrack(track, api::v1::Vote::DOWN_VOTE);
            getOut() << fmt::format("Vote revoked for track '{}' by '{}'.", track.title, track.artist) << std::endl;
        }
    }

    ShellCommandDetails Vote::getCommandDetails() const {
        ShellCommandDetails details;
        details.description = "Vote for or track of the normal queue or revoke a vote for it.";
        details.usage       = getTrigger() + " <dir>";
        details.parameterDescription["<dir>"] =
            "Specify if the track is upvoted or downvoted. Valid values are: up/revoke. [Default: up]";
        return details;
    }

}  // namespace commands::v1
