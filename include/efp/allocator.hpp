#ifndef _EFP_ALLOCATOR_HPP_
#define _EFP_ALLOCATOR_HPP_

// #if !defined(__STDC_HOSTED__)
// Define a custom allocator for freestanding environments

#include "efp/cpp_core.hpp"

namespace efp {
template<typename T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    template<typename U>
    struct rebind {
        using other = Allocator<U>;
    };

    Allocator() noexcept = default;

    template<typename U>
    Allocator(const Allocator<U>&) noexcept {}

    pointer allocate(size_type n) {
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }

    void deallocate(pointer p, size_type) {
        ::operator delete(p);
    }

    size_type max_size() const noexcept {
        return size_type(-1) / sizeof(T);
    }

    void construct(pointer p, const T& t) {
        new (p) T(t);
    }

    void destroy(pointer p) {
        p->~T();
    }
};

}  // namespace efp

// #endif

#endif