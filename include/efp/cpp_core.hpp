#ifndef CPP_CORE_HPP
#define CPP_CORE_HPP

// C++ 11 Freestanding headers specified by N3242=11-0012

#include <cstddef>  // Defines int, ptrdiff_t, and nullptr_t
#include <limits>   // Defines numeric limits
#include <cstdlib>  // Defines general utilities: memory management, program utilities, string conversions, random numbers, and more.
#include <new>  // Defines operator new and operator delete, which can be overridden for custom memory management.
#include <typeinfo>   // Defines std::type_info, which is returned by typeid.
#include <exception>  // Defines std::exception, the base class for all exceptions.
#include <initializer_list>  // Defines std::initializer_list, which is used to create a list of elements of the same type.
#include <cstdarg>      // Defines a variable argument list and macros for accessing it.
#include <type_traits>  // Defines compile-time type information.
#include <atomic>       // Defines atomic operations on data shared between threads.
#include <thread>       // Defines classes and functions for managing threads.

#if defined(__STDC_HOSTED__) && __STDC_HOSTED__ == 1

    #include <cstring>

namespace efp {
void* (*memcpy)(void* dest, const void* src, size_t size) = std::memcpy;
}

#else

namespace efp {
void* memcpy(void* dest, const void* src, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        static_cast<char*>(dest)[i] = static_cast<const char*>(src)[i];
    }
    return dest;
}
}  // namespace efp

#endif

#endif