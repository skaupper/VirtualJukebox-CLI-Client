#include "ApiCommands.h"

#include "utils/utils.h"

#include "api/v1/Api.h"
#include "exceptions/ShellException.h"


using namespace api::v1;


//
// Helper types
//

enum class RequestedQueues { ALL, NORMAL, ADMIN, CURRENT };

static std::optional<RequestedQueues> getRequestedQueues(const std::string &str) {
    if (str == "all") {
        return RequestedQueues::ALL;
    } else if (str == "normal") {
        return RequestedQueues::NORMAL;
    } else if (str == "admin") {
        return RequestedQueues::ADMIN;
    } else if (str == "current") {
        return RequestedQueues::CURRENT;
    }

    return std::nullopt;
}


//
// Helper functions
//

static void printCurrentlyPlaying(std::ostream &out, const Queues &queues) {
    if (queues.currentlyPlaying) {
        const auto &track {queues.currentlyPlaying.value()};
        out << "Currently playing: " << track.title << " - " << track.artist << std::endl;
    } else {
        out << "Nothing is currently playing" << std::endl;
    }
}

static void printNormalQueue(std::ostream &out, const Queues &queues, const size_t limit) {
    if (queues.normalQueue.size() > 0) {
        out << "Normal queue:" << std::endl;

        const auto tracksToPrint {std::min(limit, std::size(queues.normalQueue))};
        int trackRank = 1;

        std::for_each_n(std::cbegin(queues.normalQueue), tracksToPrint, [&](const auto &track) {
            out << fmt::format("  {}: {} - {}", trackRank, track.title, track.artist) << std::endl;
            ++trackRank;
        });
    } else {
        out << "Normal queue is empty." << std::endl;
    }
}

static void printAdminQueue(std::ostream &out, const Queues &queues, const size_t limit) {
    if (queues.adminQueue.size() > 0) {
        out << "Admin queue:" << std::endl;

        const auto tracksToPrint {std::min(limit, std::size(queues.adminQueue))};
        int trackRank = 1;

        std::for_each_n(std::cbegin(queues.adminQueue), tracksToPrint, [&](const auto &track) {
            out << fmt::format("  {}: {} - {}", trackRank, track.title, track.artist) << std::endl;
            ++trackRank;
        });
    } else {
        out << "Admin queue is empty." << std::endl;
    }
}

static void printRequestedQueues(std::ostream &out, const Queues &queues, const RequestedQueues reqQueues,
                                 const size_t limit) {

    switch (reqQueues) {
    case RequestedQueues::ALL:
        printCurrentlyPlaying(out, queues);
        out << std::endl;
        printAdminQueue(out, queues, limit);
        out << std::endl;
        printNormalQueue(out, queues, limit);
        break;

    case RequestedQueues::CURRENT:
        printCurrentlyPlaying(out, queues);
        break;

    case RequestedQueues::ADMIN:
        printAdminQueue(out, queues, limit);
        break;

    case RequestedQueues::NORMAL:
        printNormalQueue(out, queues, limit);
        break;
    }
}

static auto parseArgs(const std::vector<std::string> &args) {

    if (std::size(args) > 2) {
        throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_NUMBER);
    }


    RequestedQueues queueType {RequestedQueues::ALL};
    size_t limit {10};

    if (std::size(args) >= 1) {
        const auto optQueueType {getRequestedQueues(args[0])};
        if (!optQueueType) {
            throw ShellException(ShellExceptionCode::INVALID_ARGUMENT_FORMAT);
        }
        queueType = optQueueType.value();
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


//
// Actual command
//

namespace commands::v1 {

    void PrintQueues::doExecute(const std::vector<std::string> &args) {
        auto api = api::v1::Api::getInstance();

        const auto [queueType, limit] = parseArgs(args);

        auto queues = api->getCurrentQueues();
        printRequestedQueues(getOut(), queues, queueType, limit);
    }

    ShellCommandDetails PrintQueues::getCommandDetails() const {
        ShellCommandDetails details;
        details.description = "Queries and prints the contents of any queue and/or the currently playing song.";
        details.usage       = getTrigger() + " [<queue_type> [<limit>]]";
        details.parameterDescription["<queue_type>"] =
            "Determines the queue to be printed. Valid values are: all/normal/admin/current. [Default: all]";
        details.parameterDescription["<limit>"] =
            "Limits how many entries of a queue should be printed at max. [Default: 10]";
        return details;
    }

}  // namespace commands::v1
