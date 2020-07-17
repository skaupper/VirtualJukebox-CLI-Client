/*****************************************************************************/
/**
 * @file    deserializer.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of a generic deserializer function for the types used by the REST API.
 */
/*****************************************************************************/

#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "ApiTypes.h"

#include "exceptions/InvalidFormatException.h"

#include <nlohmann/json.hpp>


using json = nlohmann::json;


namespace api::v1 {

    //
    // Abstracted deserialization functions
    //

    namespace detail {

        //
        // Overloads for all known API types
        //

        void deserialize(const json &, BaseTrack &);
        void deserialize(const json &, QueueTrack &);
        void deserialize(const json &, NormalQueueTrack &);
        void deserialize(const json &, PlayingTrack &);

        void deserialize(const json &, Queues &);


        //
        // Specialization to deserialize items in a container type
        //

        template<typename T>
        void deserialize(const json &j, std::vector<T> &vec) {
            for (const json &jsonEntry : j) {
                T obj;
                deserialize(jsonEntry, obj);
                vec.emplace_back(obj);
            }
        }

    }  // namespace detail


    //
    // Function to be used by users
    //

    template<typename T>
    T deserialize(const json &j) {
        T t;
        try {
            detail::deserialize(j, t);
        } catch (const json::out_of_range &) {
            throw InvalidFormatException("An expected field could not be found in JSON object.", j.dump());
        }
        return t;
    }

}  // namespace api::v1

#endif
