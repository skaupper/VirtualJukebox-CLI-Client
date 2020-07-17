
#ifndef API_V1_PROVIDER_H
#define API_V1_PROVIDER_H

#include "api-v1.h"


namespace api::v1 {

    APIv1 *createNewApi(const std::string &address, const unsigned int port);
    APIv1 *getApi();

}  // namespace api::v1

#endif
