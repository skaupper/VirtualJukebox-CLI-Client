#include "ApiCommands.h"

#include "utils/utils.h"

#include "api/v1/Api.h"
#include "exceptions/ShellException.h"

using namespace api::v1;


//
// Helper types
//


//
// Helper functions
//

static auto parseArgs(const std::vector<std::string> &args) {

    if (std::size(args) > 2) {
        throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
    }

    QueueType queueType {QueueType::NORMAL};
    unsigned int limit {10};

    if (std::size(args) >= 1) {
        try {
            queueType = from_string<QueueType>(args[0]);
        } catch (...) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_FORMAT);
        }
    }

    if (std::size(args) >= 2) {
        const auto optLimit {sk::to_number<unsigned int>(args[1])};
        if (!optLimit) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_FORMAT);
        }
        if (optLimit.value() == 0) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_VALUE);
        }
        limit = optLimit.value();
    }

    return std::tuple {queueType, limit};
}

static std::optional<std::string> getQueryString(std::ostream &out, std::istream &in) {

    out << "Enter the query string: ";
    std::string query;
    if (!std::getline(in, query).good()) {
        return std::nullopt;
    }
    return query;
}

static std::optional<BaseTrack> selectTrack(std::ostream &out, std::istream &in, const std::vector<BaseTrack> &tracks) {
    const int width {int(std::ceil(std::log10(std::size(tracks) + 1)))};

    int trackRank = 1;
    std::for_each(std::cbegin(tracks), std::cend(tracks), [&](const auto &track) {
        out << fmt::format("{:{}}: {} - {}", trackRank, width, track.title, track.artist) << std::endl;
        ++trackRank;
    });

    // Ask the user to select a track
    out << "Which track should be added? ";

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

    return tracks[trackNumber - 1];
}


//
// Actual command
//

namespace commands::v1 {

    void AddTrack::doExecute(const std::vector<std::string> &args) {

        const auto [queueType, limit] = parseArgs(args);
        auto api                      = api::v1::getApiChecked();

        if (queueType == QueueType::ADMIN && !api->isAdmin()) {
            getOut() << "It needs an admin session to add tracks to the admin queue!" << std::endl
                     << "Try specifying an admin password while logging in." << std::endl;
            return;
        }


        // Ask the user for a query string to look up the desired song
        const auto optQuery {getQueryString(getOut(), getIn())};
        if (!optQuery) {
            return;
        }


        // Look up the desired song
        const auto tracks {api->queryTracks(optQuery.value(), limit)};


        // Ask the user to pick one of the possibilities
        const auto optSelectedTrack {selectTrack(getOut(), getIn(), tracks)};
        if (!optSelectedTrack) {
            return;
        }
        const auto selectedTrack {optSelectedTrack.value()};


        // Add the selected track to the queue
        api->addTrack(selectedTrack, queueType);
        getOut() << fmt::format("Added track '{}' by '{}' to {} queue", selectedTrack.title, selectedTrack.artist,
                                to_string(queueType))
                 << std::endl;
    }

    ShellCommandDetails AddTrack::getCommandDetails() const {
        ShellCommandDetails details;
        details.description = "Add a track to any queue. The track is selected using a query string with a subsequent "
                              "selection by the user.";
        details.usage                           = getTrigger() + " [<queue> [<limit>]]";
        details.parameterDescription["<queue>"] = "Defines in which queue the track should eventually be added. Valid "
                                                  "values are: normal/admin. [Default: normal]";
        details.parameterDescription["<limit>"] = "Limits the amount of search results to be shown. [Default: 10]";
        return details;
    }

}  // namespace commands::v1
