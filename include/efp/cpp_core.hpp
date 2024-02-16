#ifndef CPP_CORE_HPP
#define CPP_CORE_HPP

// C++ 11 Freestanding headers specified by N3242=11-0012

// <cstddef>
// <limits>
// <cstdlib>
// <new>
// <typeinfo>
// <exception>
// <initializer_list>
// <cstdarg>
// <type_traits>
// <atomic>
// <thread>

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

// // C cores
// #include <float.h>  // Macros for numeric limits
// #include <math.h>   // Common mathematical functions (e.g., sin, cos, abs).
// #include <stdarg.h>
// #include <stddef.h>  // Common macros (e.g., NULL, int) and limit constants.
// #include <stdint.h>  // Fixed-width size_tegers (e.g., uint8_t, size_t16_t, etc.)
// #include <stdio.h>   // Input and output utilities (e.g., sprintf, snprintf).
// #include <stdlib.h>  // Standard utility functions (e.g., malloc, free, atoi, itoa).
// #include <string.h>  // String handling functions (e.g., strcpy, strlen, strcat).

// // C++
// #include <cstddef>  // Defines int, ptrdiff_t, and nullptr_t
// #include <new>  // Defines operator new and operator delete, which can be overridden for custom memory management.

// // ! temp
// #include <initializer_list>
// // todo STL only
// #include <array>
// #include <iostream>
// #include <string>
// #include <vector>

#endif