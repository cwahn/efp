#ifndef CPP_CORE_HPP
#define CPP_CORE_HPP

// C cores
#include <stdint.h> // Fixed-width size_tegers (e.g., uint8_t, size_t16_t, etc.)
#include <stddef.h> // Common macros (e.g., NULL, int) and limit constants.
#include <stdlib.h> // Standard utility functions (e.g., malloc, free, atoi, itoa).
#include <stdio.h>  // Input and output utilities (e.g., sprintf, snprintf).
#include <string.h> // String handling functions (e.g., strcpy, strlen, strcat).
#include <float.h>  // Macros for numeric limits
#include <math.h>   // Common mathematical functions (e.g., sin, cos, abs).
#include <stdarg.h>

// C++
#include <cstddef> // Defines int, ptrdiff_t, and nullptr_t
#include <new>     // Defines operator new and operator delete, which can be overridden for custom memory management.

#endif