#ifndef _EFP_CPP_CORE_HPP
#define _EFP_CPP_CORE_HPP

// C++ 11 Freestanding headers specified by N3242=11-0012

#include <atomic>   // Defines atomic operations on data shared between threads.
#include <cstdarg>  // Defines a variable argument list and macros for accessing it.
#include <cstddef>  // Defines int, ptrdiff_t, and nullptr_t
#include <cstdlib>  // Defines general utilities: memory management, program utilities, string conversions, random numbers, and more.
#include <exception>  // Defines std::exception, the base class for all exceptions.
#include <initializer_list>  // Defines std::initializer_list, which is used to create a list of elements of the same type.
#include <limits>  // Defines numeric limits
#include <new>  // Defines operator new and operator delete, which can be overridden for custom memory management.
#include <thread>       // Defines classes and functions for managing threads.
#include <type_traits>  // Defines compile-time type information.
#include <typeinfo>     // Defines std::type_info, which is returned by typeid.

// _memcpy

#if defined(__STDC_HOSTED__) && __STDC_HOSTED__ == 1

    #include <cstring>

namespace efp {
// Use a function alias for clarity and avoid direct assignment to avoid potential confusion.
inline void* _memcpy(void* dest, const void* src, size_t size) {
    return std::memcpy(dest, src, size);
}
}  // namespace efp

#else

namespace efp {
// Custom memcpy implementation for freestanding environments.
// Added extern "C" to ensure C linkage for compatibility with C libraries or code.
extern "C" void* _memcpy(void* dest, const void* src, size_t size) {
    auto* d = static_cast<char*>(dest);
    const auto* s = static_cast<const char*>(src);
    for (size_t i = 0; i < size; ++i) {
        d[i] = s[i];
    }
    return dest;
}
}  // namespace efp

#endif

// Excptions

#if defined(__STDC_HOSTED__) && __STDC_HOSTED__ == 1

    #include <stdexcept>
// Rename std::excpetion to efp::exception

namespace efp {
    
using Exception = std::exception;
using BadAlloc = std::bad_alloc;
using BadArrayNewLength = std::bad_array_new_length;
using BadCast = std::bad_cast;
using BadException = std::bad_exception;
using BadFunctionCall = std::bad_function_call;
using BadTypeId = std::bad_typeid;
using LogicError = std::logic_error;
using DomainError = std::domain_error;
using InvalidArgument = std::invalid_argument;
using LengthError = std::length_error;
using OutOfRange = std::out_of_range;
using RuntimeError = std::runtime_error;
using OverflowError = std::overflow_error;
using RangeError = std::range_error;
using UnderflowError = std::underflow_error;

}  // namespace efp

#else

#endif

#endif