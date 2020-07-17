/*****************************************************************************/
/**
 * @file    http-status.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of HTTP status codes.
 */
/*****************************************************************************/

#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

namespace sk {
    enum class HttpStatus {
        OK           = 200,
        NOT_FOUND    = 400,
        UNAUTHORIZED = 401,
        // TODO
    };
}

#endif /* HTTP_STATUS_H */
