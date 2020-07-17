#include "api-v1-provider.h"


namespace api::v1 {

    static std::unique_ptr<APIv1> apiInstance {nullptr};


    APIv1 *createNewApi(const std::string &address, const unsigned int port) {
        apiInstance = std::make_unique<APIv1>(address, port);
        return getApi();
    }

    APIv1 *getApi() { return apiInstance.get(); }

}  // namespace api::v1
