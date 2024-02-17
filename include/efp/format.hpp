#ifndef EFP_FORMAT_HPP_
#define EFP_FORMAT_HPP_

#define FMT_HEADER_ONLY
// #include "efp/fmt_original/core.h"
#include "efp/fmt/core.h"


namespace efp {

template<typename... Args>
String format(const String& fmt, Args&&... args) {
    return fmt::format(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void print(const String& fmt, Args&&... args) {
    fmt::print(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void println(const String& fmt, Args&&... args) {
    fmt::println(fmt, std::forward<Args>(args)...);
}

// todo styled print

}  // namespace efp
#endif