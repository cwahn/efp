#ifndef EFP_FORMAT_HPP_
#define EFP_FORMAT_HPP_

#define FMT_HEADER_ONLY
// #include "efp/fmt_original/core.h"
#include "efp/fmt/core.h"

namespace efp {

template<typename... Args>
using FormatString = fmt::format_string<Args...>;

// template<typename... Args>
// inline String format(const String& fmt, Args&&... args) {
//     return fmt::format(fmt, efp::forward<Args>(args)...);
// }

template<typename... Args>
inline auto format(FormatString<Args...> fmt, Args&&... args) -> String {
    return fmt::vformat(fmt, fmt::make_format_args(args...));
}

// template<typename... Args>
// inline void print(const String& fmt, Args&&... args) {
//     fmt::print(fmt, efp::forward<Args>(args)...);
// }

template<typename... Args>
inline void print(FormatString<Args...> fmt, Args&&... args) {
    const auto& vargs = fmt::make_format_args(args...);
    return fmt::detail::is_utf8() ? fmt::vprint(fmt, vargs)
                                  : fmt::detail::vprint_mojibake(stdout, fmt, vargs);
}

template<typename... Args>
inline void print(std::FILE* f, fmt::format_string<Args...> fmt, Args&&... args) {
    const auto& vargs = fmt::make_format_args(args...);
    return fmt::detail::is_utf8() ? fmt::vprint(f, fmt, vargs)
                                  : fmt::detail::vprint_mojibake(f, fmt, vargs);
}

// template<typename... Args>
// inline void println(const String& fmt, Args&&... args) {
//     fmt::println(fmt, efp::forward<Args>(args)...);
// }

template<typename... Args>
inline void println(FormatString<Args...> fmt, Args&&... args) {
    return fmt::println(stdout, fmt, efp::forward<Args>(args)...);
}

template<typename... Args>
inline void println(std::FILE* f, fmt::format_string<Args...> fmt, Args&&... args) {
    return fmt::print(f, "{}\n", fmt::format(fmt, efp::forward<Args>(args)...));
}

// todo styled print

}  // namespace efp
#endif