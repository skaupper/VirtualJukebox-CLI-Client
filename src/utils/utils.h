#ifndef SK_UTILS_H
#define SK_UTILS_H


#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>

#include <fmt/format.h>


namespace sk {

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    std::optional<T> to_number(const std::string &valStr) {
        T valNum;
        size_t idx;

        try {
            // Use the most appropriate conversion function for the type T
            if constexpr (sizeof(T) <= sizeof(int)) {
                if constexpr (std::is_signed_v<T>) {
                    valNum = std::stoi(valStr, &idx);
                } else {
                    valNum = std::stoul(valStr, &idx);
                }
            } else {
                if constexpr (std::is_signed_v<T>) {
                    valNum = std::stoll(valStr, &idx);
                } else {
                    valNum = std::stoull(valStr, &idx);
                }
            }

            // For unsigned types of T make sure the string does not contain a negative number
            if constexpr (std::is_unsigned_v<T>) {
                if (!valStr.empty() && valStr[0] == '-') {
                    return std::nullopt;
                }
            }
        } catch (const std::invalid_argument &) {
            return std::nullopt;
        }

        // Make sure that the whole string got processed
        if (idx != valStr.size()) {
            return std::nullopt;
        }
        return valNum;
    }

}  // namespace sk


template<typename T>
std::ostream &operator<<(std::ostream &out, const std::optional<T> &option) {
    if (option) {
        out << option.value();
    } else {
        out << "<empty>";
    }
    return out;
}

template<typename T>
struct fmt::formatter<std::optional<T>> : formatter<T> {
    template<typename FormatContext>
    auto format(std::optional<T> opt, FormatContext &ctx) {
        if (opt) {
            return formatter<T>::format(opt.value(), ctx);
        } else {
            return formatter::format("<empty>", ctx);
        }
    }
};


#endif /* SK_UTILS_H */
