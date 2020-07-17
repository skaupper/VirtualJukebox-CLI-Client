#ifndef SK_UTILS_H
#define SK_UTILS_H


#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>

#include <fmt/format.h>


namespace sk {

    template<typename T>
    inline std::optional<T> to_number(const std::string &valStr) {
        T valNum;
        size_t idx;

        try {
            if constexpr (std::is_same_v<T, int>) {
                valNum = std::stoi(valStr, &idx);
            } else if constexpr (std::is_same_v<T, unsigned int>) {
                valNum = std::stoul(valStr, &idx);
            }
        } catch (const std::invalid_argument &) {
            return std::nullopt;
        }

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
