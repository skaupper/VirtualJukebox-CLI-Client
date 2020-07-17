#include "Api.h"


namespace api::v1 {

    static std::unique_ptr<Api> apiInstance {nullptr};


    Api *createNewApi(const std::string &address, const unsigned int port) {
        apiInstance = std::make_unique<Api>(address, port);
        return getApi();
    }

    Api *getApi() { return apiInstance.get(); }

    Api *getApiChecked() {
        auto api = getApi();
        if (!api) {
            throw std::runtime_error("No API instance has been generated yet");
        }
        return api;
    }

}  // namespace api::v1
