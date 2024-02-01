#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

// ct_capacity is compile time bound of length. It does not mean safety of access.
// However, actual capacity does means the length of memory safe to access.

#include "efp/cpp_core.hpp"
#include "efp/meta.hpp"
#include "efp/trait.hpp"

// todo Move and copy assigment operator sort out

namespace efp {

template<typename A, size_t ct_size>
class Array {
  public:
    using Element = A;
    using CtSize = Size<ct_size>;
    using CtCapacity = Size<ct_size>;

    Array() {
        // By definition all of the data in Array should be valid
        for (size_t i = 0; i < ct_size; ++i) {
            new (_data + i) Element {};
        }
    }

    Array(const Array& other) {
        for (size_t i = 0; i < ct_size; ++i) {
            new (_data + i) Element {other._data[i]};
        }
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            for (size_t i = 0; i < ct_size; ++i) {
                (_data + i)->~Element();                   // Destroy each element
                new (_data + i) Element {other._data[i]};  // Copy-construct each element
            }
        }
        return *this;
    }

    Array(Array&& other) noexcept {
        for (size_t i = 0; i < ct_size; ++i) {
            new (_data + i) Element {std::move(other._data[i])};  // Move-construct each element
        }
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < ct_size; ++i) {
                (_data + i)->~Element();                              // Destroy each element
                new (_data + i) Element {std::move(other._data[i])};  // Move-construct each element
            }
        }
        return *this;
    }

    ~Array() {
        for (size_t i = 0; i < ct_size; ++i) {
            (_data + i)->~Element();
        }
    }

    template<typename... Arg>
    Array(const Arg&... args) {
        static_assert(
            sizeof...(args) == ct_size,
            "Array::Array: number of arguments must be equal to ct_size"
        );
        size_t index = 0;
        _construct_elements(index, args...);
    }

    Element& operator[](size_t index) {
        return _data[index];
    }

    const Element& operator[](size_t index) const {
        return _data[index];
    }

    bool operator==(const Array& other) const {
        for (size_t i = 0; i < ct_size; ++i) {
            if (_data[i] != other._data[i]) {
                return false;
            }
        }
        return true;
    }

    size_t size() const {
        return ct_size;
    }

    size_t capacity() const {
        return ct_size;
    }

    void resize(size_t length) {
        if (length != ct_size) {
            throw std::runtime_error("Array::resize: length must be equal to ct_size");
        }
    }

    void reserve(size_t capacity) {
        if (capacity > ct_size) {
            throw std::runtime_error(
                "Array::reserve: capacity must be less than or equal to ct_size"
            );
        }
    }

    const Element* data() const {
        return _data;
    }

    Element* data() {
        return _data;
    }

    Element* begin() {
        return _data;
    }

    const Element* begin() const {
        return _data;
    }

    Element* end() {
        return _data + ct_size;
    }

    const Element* end() const {
        return _data + ct_size;
    }

    bool empty() const {
        return ct_size == 0;
    }

  private:
    template<typename Head, typename... Tail>
    inline void _construct_elements(size_t& index, const Head& head, const Tail&... tail) {
        new (_data + index++) Element {head};
        _construct_elements(index, tail...);
    }

    template<typename Last>
    inline void _construct_elements(size_t& index, const Last& last) {
        new (_data + index) Element {last};
    }

    RawStorage<A, ct_size> _data;
};

template<typename A, size_t n>
struct ElementImpl<Array<A, n>> {
    using Type = A;
};

template<typename A, size_t n>
struct CtSizeImpl<Array<A, n>> {
    using Type = Size<n>;
};

template<typename A, size_t n>
struct CtCapacityImpl<Array<A, n>> {
    using Type = Size<n>;
};

template<typename A, size_t n>
constexpr auto length(const Array<A, n>& as) -> Size<n> {
    return Size<n> {};
}

template<typename A, size_t n>
constexpr auto nth(size_t i, const Array<A, n>& as) -> const A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto nth(size_t i, Array<A, n>& as) -> A& {
    return as[i];
}

// static_assert(IsSame<decltype(nth(0, Array<int, 2>{0, 1, 2})), int &>::value, "Assert nth");
// DebugType<decltype(nth(0, Array<int, 2>{0, 1, 2}))> _;

template<typename A, size_t n>
constexpr auto data(const Array<A, n>& as) -> const A* {
    return as.data();
}

template<typename A, size_t n>
constexpr auto data(Array<A, n>& as) -> A* {
    return as.data();
}

template<typename A, size_t ct_capacity>
class ArrVec {
  public:
    using Element = A;
    using CtSize = Size<dyn>;
    using CtCapacity = Size<ct_capacity>;

    ArrVec() : _size {0} {
        // By definition none of the data in ArrVec of size 0 should be valid
    }

    // ArrVec(const ArrVec& other) : _size {other._size} {
    //     for (size_t i = 0; i < _size; ++i) {
    //         new (&_data[i]) Element(other._data[i]);
    //     }
    // }
    ArrVec(const ArrVec& other) : _size {other._size} {
        for (size_t i = 0; i < _size; ++i) {
            new (_data + i) Element {other._data[i]};
        }
    }

    // ArrVec& operator=(const ArrVec& other) {
    //     if (this != &other) {
    //         resize(other.size());
    //         for (size_t i = 0; i < _size; ++i) {
    //             _data[i] = other._data[i];
    //         }
    //     }
    //     return *this;
    // }

    ArrVec& operator=(const ArrVec& other) {
        if (this != &other) {
            for (size_t i = 0; i < _size; ++i) {
                (_data + i)->~Element();
            }

            _size = other._size;

            for (size_t i = 0; i < _size; ++i) {
                new (_data + i) Element {other._data[i]};  // Copy-construct each element
            }
        }
        return *this;
    }

    // ArrVec(ArrVec&& other) : _size {other._size} {
    //     other._size = 0;
    //     for (size_t i = 0; i < _size; ++i) {
    //         _data[i] = efp::move(other._data[i]);
    //     }
    // }

    ArrVec(ArrVec&& other) noexcept : _size {other._size} {
        other._size = 0;
        for (size_t i = 0; i < _size; ++i) {
            new (_data + i) Element {std::move(other._data[i])};
        }
    }

    // ArrVec& operator=(ArrVec&& other) noexcept {
    //     if (this != &other) {
    //         // Destroy existing elements
    //         for (size_t i = 0; i < _size; ++i) {
    //             _data[i].~Element();
    //         }

    //         // Move data from the source object
    //         _size = other._size;
    //         for (size_t i = 0; i < _size; ++i) {
    //             new (&_data[i]) Element(efp::move(other._data[i]));
    //         }

    //         // Reset the source object
    //         other._size = 0;
    //     }
    //     return *this;
    // }

    ArrVec& operator=(ArrVec&& other) noexcept {
        if (this != &other) {
            // Destroy existing elements
            for (size_t i = 0; i < _size; ++i) {
                (_data + i)->~Element();
            }

            // Move data from the source object
            _size = other._size;
            for (size_t i = 0; i < _size; ++i) {
                new (_data + i) Element {std::move(other._data[i])};
            }

            // Reset the source object
            other._size = 0;
        }
        return *this;
    }

    // ~ArrVec() {
    //     for (size_t i = 0; i < _size; ++i) {
    //         _data[i].~Element();
    //     }
    // }

    ~ArrVec() {
        for (size_t i = 0; i < _size; ++i) {
            (_data + i)->~Element();
        }
    }

    // Constructor from array
    // template<size_t ct_size_, typename = EnableIf<ct_capacity >= ct_size_, void>>
    // ArrVec(const ArrVec<Element, ct_size_>& as) : _size(ct_size_) {
    //     for (size_t i = 0; i < ct_size_; ++i) {
    //         new (&_data[i]) Element(as[i]);
    //     }
    // }

    // Constructor from array
    template<size_t ct_size_, typename = EnableIf<ct_capacity >= ct_size_, void>>
    ArrVec(const ArrVec<Element, ct_size_>& as) : _size(ct_size_) {
        for (size_t i = 0; i < ct_size_; ++i) {
            new (_data + i) Element {as[i]};
        }
    }

    // template<typename... Arg>
    // ArrVec(const Arg&... args) : _data {args...}, _size(sizeof...(args)) {}

    template<typename... Arg>
    ArrVec(const Arg&... args) : _size(sizeof...(args)) {
        static_assert(
            sizeof...(args) <= ct_capacity,
            "ArrVec::ArrVec: number of arguments must be less than or equal to ct_capacity"
        );
        size_t index = 0;
        _construct_elements(index, args...);
    }

    Element& operator[](size_t index) {
        return _data[index];
    }

    const Element& operator[](size_t index) const {
        return _data[index];
    }

    bool operator==(const ArrVec& other) const {
        if (_size != other._size) {
            return false;
        }

        for (size_t i = 0; i < _size; ++i) {
            if (_data[i] != other._data[i]) {
                return false;
            }
        }

        return true;
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return ct_capacity;
    }

    void resize(size_t length) {
        if (length > ct_capacity || length < 0) {
            throw std::runtime_error(
                "ArrVec::resize: length must be less than or equal to ct_capacity"
            );
        }

        _size = length;
    }

    void reserve(size_t capacity) {
        if (capacity > ct_capacity) {
            throw std::runtime_error(
                "ArrVec::reserve: capacity must be less than or equal to ct_capacity"
            );
        }
    }

    void push_back(const Element& value) {
        if (_size >= ct_capacity) {
            throw std::runtime_error(
                "ArrVec::push_back: size must be less than or equal to ct_capacity"
            );
        } else {
            new (_data + _size++) Element {value};
        }
    }

    // void push_back(Element&& value) {
    //     if (_size >= ct_capacity) {
    //         throw std::runtime_error(
    //             "ArrVec::push_back: size must be less than or equal to ct_capacity"
    //         );
    //     } else {
    //         new (&_data[_size]) Element(efp::move(value));
    //         ++_size;
    //     }
    // }

    void push_back(Element&& value) {
        if (_size >= ct_capacity) {
            throw std::runtime_error(
                "ArrVec::push_back: size must be less than or equal to ct_capacity"
            );
        } else {
            new (_data + _size++) Element {std::move(value)};
        }
    }

    // void insert(size_t index, const Element& value) {
    //     if (index < 0 || index > _size || _size == ct_capacity) {
    //         throw std::runtime_error(
    //             "ArrVec::insert: index must be less than or equal to size and size must be less than or equal to ct_capacity"
    //         );
    //     }

    //     for (size_t i = _size; i > index; --i) {
    //         new (&_data[i]) Element(efp::move(_data[i - 1]));
    //         _data[i - 1].~Element();
    //     }

    //     new (&_data[index]) Element(value);

    //     ++_size;
    // }

    void insert(size_t index, const Element& value) {
        if (index < 0 || index > _size || _size == ct_capacity) {
            throw std::runtime_error(
                "ArrVec::insert: index must be less than or equal to size and size must be less than or equal to ct_capacity"
            );
        }

        for (size_t i = _size; i > index; --i) {
            new (_data + i) Element(efp::move(_data[i - 1]));
            (_data + i - 1)->~Element();
        }

        // new (&_data[index]) Element(value);
        new (_data + index) Element(value);

        ++_size;
    }

    // void pop_back() {
    //     if (_size == 0) {
    //         std::runtime_error("ArrVec::pop_back: size must be greater than 0");
    //     }

    //     _data[_size - 1].~Element();
    //     --_size;
    // }

    void pop_back() {
        if (_size == 0) {
            throw std::runtime_error("ArrVec::pop_back: size must be greater than 0");
        }

        (_data + _size-- - 1)->~Element();
    }

    // void clear() {
    //     for (size_t i = 0; i < _size; ++i) {
    //         _data[i].~Element();
    //     }
    //     _size = 0;
    // }

    void clear() {
        for (size_t i = 0; i < _size; ++i) {
            (_data + i)->~Element();
        }
        _size = 0;
    }

    // void erase(size_t index) {
    //     if (index < 0 || index >= _size) {
    //         throw std::runtime_error("ArrVec::erase: index must be less than or equal to size");
    //     }
    //     _data[index].~Element();
    //     for (size_t i = index; i < _size - 1; ++i) {
    //         new (&_data[i]) Element(efp::move(_data[i + 1]));
    //         _data[i + 1].~Element();
    //     }
    //     --_size;
    // }

    void erase(size_t index) {
        if (index < 0 || index >= _size) {
            throw std::runtime_error("ArrVec::erase: index must be less than or equal to size");
        }

        (_data + index)->~Element();

        for (size_t i = index; i < _size - 1; ++i) {
            new (_data + i) Element {efp::move(_data[i + 1])};
            (_data + i + 1)->~Element();
        }
        --_size;
    }

    const Element* data() const {
        return _data;
    }

    Element* data() {
        return _data;
    }

    Element* begin() {
        return _data;
    }

    const Element* begin() const {
        return _data;
    }

    Element* end() {
        return _data + _size;
    }

    const Element* end() const {
        return _data + _size;
    }

    bool empty() const {
        return _size == 0;
    }

  private:
    template<typename Head, typename... Tail>
    void _construct_elements(size_t& index, const Head& head, const Tail&... tail) {
        new (_data + index++) Element {head};
        _construct_elements(index, tail...);
    }

    template<typename Last>
    void _construct_elements(size_t& index, const Last& last) {
        new (_data + index++) Element {last};
    }

    // Element _data[ct_capacity];
    RawStorage<Element, ct_capacity> _data;
    size_t _size;
};

template<typename A, size_t n>
struct ElementImpl<ArrVec<A, n>> {
    using Type = A;
};

template<typename A, size_t n>
struct CtSizeImpl<ArrVec<A, n>> {
    using Type = Size<dyn>;
};

template<typename A, size_t n>
struct CtCapacityImpl<ArrVec<A, n>> {
    using Type = Size<n>;
};

template<typename A, size_t n>
constexpr auto length(const ArrVec<A, n>& as) -> size_t {
    return as.size();
}

template<typename A, size_t n>
constexpr auto nth(size_t i, const ArrVec<A, n>& as) -> const A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto nth(size_t i, ArrVec<A, n>& as) -> A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto data(const ArrVec<A, n>& as) -> const A* {
    return as.data();
}

template<typename A, size_t n>
constexpr auto data(ArrVec<A, n>& as) -> A* {
    return as.data();
}

namespace detail {
    template<typename A>
    class VectorBase {
      public:
        using Element = A;
        using CtSize = Size<dyn>;
        using CtCapacity = Size<dyn>;

        VectorBase() : _data {nullptr}, _size {0}, _capacity {0} {}

        VectorBase(const VectorBase& other)
            : _data {static_cast<Element*>(::operator new[](other._size * sizeof(Element)))},
              _size {other._size}, _capacity {other._size} {
            if (other._data) {
                for (size_t i = 0; i < _size; ++i) {
                    new (_data + i) Element {other._data[i]};
                }
            }
        }

        // todo copy_and_swap
        VectorBase& operator=(const VectorBase& other) noexcept {
            if (this != &other) {
                for (size_t i = 0; i < _size; ++i) {
                    _data[i].~Element();
                }

                if (_capacity < other._size) {
                    resize(other._size);
                }

                for (size_t i = 0; i < other._size; ++i) {
                    new (_data + i) Element {other._data[i]};
                }
            }
            return *this;
        }

        VectorBase(VectorBase&& other) noexcept
            : _data {other._data}, _size {other._size}, _capacity {other._capacity} {
            other._data = nullptr;
            other._capacity = 0;
            other._size = 0;
        }

        VectorBase& operator=(VectorBase&& other) noexcept {
            if (this != &other) {
                // Destroy existing elements and deallocate memory
                for (size_t i = 0; i < _size; ++i) {
                    _data[i].~Element();
                }
                ::operator delete[](_data);

                _data = other._data;
                _size = other._size;
                _capacity = other._capacity;

                other._data = nullptr;
                other._size = 0;
                other._capacity = 0;
            }
            return *this;
        }

        template<typename... Args>
        VectorBase(const Args&... args)
            : _data {static_cast<Element*>(::operator new[](sizeof...(args) * sizeof(Element)))},
              _capacity(sizeof...(args)), _size(sizeof...(args)) {
            size_t index = 0;
            _construct_elements(index, args...);
        }

        // Constructor from Array
        template<size_t ct_size_>
        VectorBase(const Array<Element, ct_size_>& as)
            : _data {::operator new[](ct_size_ * sizeof(Element))}, _size(ct_size_),
              _capacity(ct_size_) {
            for (size_t i = 0; i < _size; ++i) {
                new (_data + i) Element {as[i]};
            }
        }

        // Constructor from ArrVec
        template<size_t ct_cap_>
        VectorBase(const ArrVec<Element, ct_cap_>& as)
            : _data {::operator new[](length(as) * sizeof(Element))}, _size(length(as)),
              _capacity(ct_cap_) {
            for (size_t i = 0; i < _size; ++i) {
                new (_data + i) Element {as[i]};
            }
        }

        ~VectorBase() {
            if (_data) {
                for (size_t i = 0; i < _size; ++i) {
                    (_data + i)->~Element();
                }
                ::operator delete[](_data);
                _data = nullptr;
            }
        }

        Element& operator[](size_t index) {
            return _data[index];
        }

        const Element& operator[](size_t index) const {
            return _data[index];
        }

        bool operator==(const VectorBase& other) const {
            if (_size != other._size) {
                return false;
            }

            for (size_t i = 0; i < _size; ++i) {
                if (_data[i] != other._data[i]) {
                    return false;
                }
            }

            return true;
        }

        size_t size() const {
            return _size;
        }

        size_t capacity() const {
            return _capacity;
        }

        void resize(size_t length) {
            if (length < 0) {
                throw std::runtime_error(
                    "VectorBase::resize: length must be greater than or equal to 0"
                );
            }

            if (length > _capacity) {
                reserve(length);
            }

            _size = length;
        }

        void reserve(size_t new_capacity) {
            if (new_capacity > _capacity) {
                Element* new_data =
                    static_cast<Element*>(::operator new[](new_capacity * sizeof(Element)));

                for (size_t i = 0; i < _size; ++i) {
                    new (new_data + i) Element {std::move(_data[i])};
                }

                ::operator delete[](_data);

                _data = new_data;
                _capacity = new_capacity;
            }
        }

        void push_back(const Element& value) {
            if (_size >= _capacity) {
                reserve(_capacity == 0 ? 1 : 2 * _capacity);
            }

            new (_data + _size) Element {value};
            ++_size;
        }

        void push_back(Element&& value) {
            if (_size >= _capacity) {
                reserve(_capacity == 0 ? 1 : 2 * _capacity);
            }

            new (_data + _size) Element {std::move(value)};
            ++_size;
        }

        void insert(size_t index, const Element& value) {
            if (index < 0 || index > _size) {
                throw std::runtime_error(
                    "VectorBase::insert: index must be less than or equal to size"
                );
            }

            if (_size >= _capacity) {
                reserve(_capacity == 0 ? 1 : 2 * _capacity);
            }

            for (size_t i = _size; i > index; --i) {
                _data[i] = efp::move(_data[i - 1]);
            }

            new (_data + index) Element {value};
            ++_size;
        }

        void erase(size_t index) {
            if (index < 0 || index >= _size) {
                throw std::runtime_error(
                    "VectorBase::erase: index must be less than or equal to size"
                );
            }

            (_data + index)->~Element();

            for (size_t i = index; i < _size - 1; ++i) {
                _data[i] = std::move(_data[i + 1]);
            }

            --_size;
        }

        void clear() {
            for (size_t i = 0; i < _size; ++i) {
                _data[i].~Element();
            }
            _size = 0;
        }

        void pop_back() {
            if (_size == 0) {
                throw std::runtime_error("VectorBase::pop_back: size must be greater than 0");
            }
            _data[_size - 1].~Element();
            --_size;
        }

        const Element* data() const {
            return _data;
        }

        Element* data() {
            return _data;
        }

        Element* begin() {
            return _data;
        }

        const Element* begin() const {
            return _data;
        }

        Element* end() {
            return _data + _size;
        }

        const Element* end() const {
            return _data + _size;
        }

        bool empty() const {
            return _size == 0;
        }

      protected:
        template<typename Head, typename... Tail>
        void _construct_elements(size_t& index, const Head& head, const Tail&... tail) {
            new (_data + index++) Element {head};
            _construct_elements(index, tail...);
        }

        template<typename Last>
        void _construct_elements(size_t& index, const Last& last) {
            new (_data + index++) Element {last};
        }

        Element* _data;
        size_t _size;
        size_t _capacity;
    };
}  // namespace detail

template<typename A>
class Vector: public detail::VectorBase<A> {
  public:
    using Base = detail::VectorBase<A>;
    using Base::Base;

    bool operator==(const Vector& other) const {
        return Base::operator==(other);
    }
};

template<typename A>
struct ElementImpl<Vector<A>> {
    using Type = A;
};

template<typename A>
struct CtSizeImpl<Vector<A>> {
    using Type = Size<dyn>;
};

template<typename A>
struct CtCapacityImpl<Vector<A>> {
    using Type = Size<dyn>;
};

template<typename A>
constexpr auto length(const Vector<A>& as) -> size_t {
    return as.size();
}

template<typename A>
constexpr auto nth(size_t i, const Vector<A>& as) -> const A& {
    return as[i];
}

template<typename A>
constexpr auto nth(size_t i, Vector<A>& as) -> A& {
    return as[i];
}

template<typename A>
constexpr auto data(const Vector<A>& as) -> const A* {
    return as.data();
}

template<typename A>
constexpr auto data(Vector<A>& as) -> A* {
    return as.data();
}

template<typename A, size_t ct_size>
class ArrayView {
  public:
    using Element = A;
    using CtSize = Size<ct_size>;
    using CtCapacity = Size<ct_size>;

    ArrayView() : _data(nullptr) {}

    // ! length will not be used
    ArrayView(Element* data, size_t length = Size<ct_size> {}) : _data(data) {
        // Ensure that data is not nullptr for a non-empty view.
        if (ct_size > 0 && _data == nullptr) {
            throw std::runtime_error(
                "ArrayView::ArrayView: data must not be nullptr for a non-empty view"
            );
        }
    }

    Element& operator[](size_t index) {
        return _data[index];
    }

    const Element& operator[](size_t index) const {
        return _data[index];
    }

    bool operator==(const ArrayView& other) const {
        return _data == other._data;
    }

    size_t size() const {
        return ct_size;
    }

    size_t capacity() const {
        return ct_size;
    }

    void resize(size_t length) {
        if (length > ct_size || length < 0) {
            throw std::runtime_error(
                "ArrayView::resize: length must be less than or equal to ct_size and greater than or equal to 0"
            );
        }
    }

    void reserve(size_t capacity) {
        if (capacity > ct_size) {
            throw std::runtime_error(
                "ArrayView::reserve: capacity must be less than or equal to ct_size"
            );
        }
    }

    const Element* data() const {
        return _data;
    }

    Element* data() {
        return _data;
    }

    Element* begin() {
        return _data;
    }

    const Element* begin() const {
        return _data;
    }

    Element* end() {
        return _data + ct_size;
    }

    const Element* end() const {
        return _data + ct_size;
    }

    bool empty() const {
        return ct_size == 0;
    }

  private:
    Element* _data;
};

template<typename A, size_t n>
struct ElementImpl<ArrayView<A, n>> {
    using Type = A;
};

template<typename A, size_t n>
struct CtSizeImpl<ArrayView<A, n>> {
    using Type = Size<n>;
};

template<typename A, size_t n>
struct CtCapacityImpl<ArrayView<A, n>> {
    using Type = Size<n>;
};

template<typename A, size_t n>
constexpr auto length(const ArrayView<A, n>& as) -> Size<n> {
    return Size<n> {};
}

template<typename A, size_t n>
constexpr auto nth(size_t i, const ArrayView<A, n>& as) -> const A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto nth(size_t i, ArrayView<A, n>& as) -> A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto data(const ArrayView<A, n>& as) -> const A* {
    return as.data();
}

template<typename A, size_t n>
constexpr auto data(ArrayView<A, n>& as) -> A* {
    return as.data();
}

template<typename A, size_t ct_capacity>
class ArrVecView {
  public:
    using Element = A;
    using CtSize = Size<dyn>;
    using CtCapacity = Size<ct_capacity>;

    ArrVecView() : _data(nullptr), _size(0) {}

    ArrVecView(Element* data, size_t size) : _data(data), _size(size) {
        // Ensure that data is not nullptr for a non-empty view.
        if (size > 0 && _data == nullptr) {
            throw std::runtime_error(
                "ArrVecView::ArrVecView: data must not be nullptr for a non-empty view"
            );
        }
    }

    // Constructor from ArrayView
    template<size_t ct_size_>
    ArrVecView(const ArrayView<Element, ct_size_>& as) : _data(as.data()), _size(length(as)) {}

    ArrVecView& operator=(const ArrVecView& other) {
        if (this != &other) {
            _data = other._data;
            _size = other._size;
        }
        return *this;
    }

    Element& operator[](size_t index) {
        return _data[index];
    }

    const Element& operator[](size_t index) const {
        return _data[index];
    }

    bool operator==(const ArrVecView& other) const {
        return (_data == other._data) && (_size == other._size);
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return ct_capacity;
    }

    const Element* data() const {
        return _data;
    }

    Element* data() {
        return _data;
    }

    Element* begin() {
        return _data;
    }

    const Element* begin() const {
        return _data;
    }

    Element* end() {
        return _data + _size;
    }

    const Element* end() const {
        return _data + _size;
    }

    bool empty() const {
        return _size == 0;
    }

  private:
    Element* _data;
    size_t _size;
};

template<typename A, size_t n>
struct ElementImpl<ArrVecView<A, n>> {
    using Type = A;
};

template<typename A, size_t n>
struct CtSizeImpl<ArrVecView<A, n>> {
    using Type = Size<dyn>;
};

template<typename A, size_t n>
struct CtCapacityImpl<ArrVecView<A, n>> {
    using Type = Size<n>;
};

template<typename A, size_t n>
constexpr auto length(const ArrVecView<A, n>& as) -> size_t {
    return as.size();
}

template<typename A, size_t n>
constexpr auto nth(size_t i, const ArrVecView<A, n>& as) -> const A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto nth(size_t i, ArrVecView<A, n>& as) -> A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto data(const ArrVecView<A, n>& as) -> const A* {
    return as.data();
}

template<typename A, size_t n>
constexpr auto data(ArrVecView<A, n>& as) -> A* {
    return as.data();
}

template<typename A>
class VectorView {
  public:
    using Element = A;
    using CtSize = Size<dyn>;
    using CtCapacity = Size<dyn>;

    VectorView() : _data {nullptr}, _size {0}, _capacity {0} {}

    VectorView(Element* data, size_t size) : _data(data), _size(size), _capacity(size) {
        // Ensure that data is not nullptr for a non-empty view.
        if (size > 0 && _data == nullptr) {
            throw std::runtime_error(
                "VectorView::VectorView: data must not be nullptr for a non-empty view"
            );
        }
    }

    // Constructor from ArrayView
    template<size_t ct_size_>
    VectorView(const ArrayView<Element, ct_size_>& as) : _data(as.data()), _size(length(as)) {}

    // Constructor from ArrVecView
    template<size_t ct_cap_>
    VectorView(const ArrVecView<Element, ct_cap_>& as) : _data(as.data()), _size(length(as)) {}

    VectorView& operator=(const VectorView& other) {
        if (this != &other) {
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
        }
        return *this;
    }

    Element& operator[](size_t index) {
        return _data[index];
    }

    const Element& operator[](size_t index) const {
        return _data[index];
    }

    bool operator==(const VectorView& other) const {
        return (_data == other._data) && (_size == other._size) && (_capacity == other._capacity);
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _capacity;
    }

    const Element* data() const {
        return _data;
    }

    Element* data() {
        return _data;
    }

    Element* begin() {
        return _data;
    }

    const Element* begin() const {
        return _data;
    }

    Element* end() {
        return _data + _size;
    }

    const Element* end() const {
        return _data + _size;
    }

    bool empty() const {
        return _size == 0;
    }

  private:
    Element* _data;
    size_t _size;
    size_t _capacity;
};

// template <typename A>
// using VectorView = SequenceView<A, dyn, dyn>;

template<typename A>
struct ElementImpl<VectorView<A>> {
    using Type = A;
};

template<typename A>
struct CtSizeImpl<VectorView<A>> {
    using Type = Size<dyn>;
};

template<typename A>
struct CtCapacityImpl<VectorView<A>> {
    using Type = Size<dyn>;
};

template<typename A>
constexpr auto length(const VectorView<A>& as) -> size_t {
    return as.size();
}

template<typename A>
constexpr auto nth(size_t i, const VectorView<A>& as) -> const A& {
    return as[i];
}

template<typename A>
constexpr auto nth(size_t i, VectorView<A>& as) -> A& {
    return as[i];
}

template<typename A>
constexpr auto data(const VectorView<A>& as) -> const A* {
    return as.data();
}

template<typename A>
constexpr auto data(VectorView<A>& as) -> A* {
    return as.data();
}

// todo STL only

template<typename A>
auto operator<<(std::ostream& os, const A& seq)
    -> EnableIf<IsSequence<A>::value && !IsSame<A, std::string>::value, std::ostream&> {
    static_assert(IsSequence<A>(), "Argument should be an instance of Sequence trait.");

    // ? Interesting. Automatically consider it as VectorStream?
    os << "{ ";
    for (size_t i = 0; i < seq.size(); ++i) {
        os << seq[i];
        if (i != seq.size() - 1) {
            os << ", ";
        }
    }
    os << " }";
    return os;
}

// Sequence trait implementation for std::array

template<typename A, size_t n>
struct ElementImpl<std::array<A, n>> {
    using Type = A;
};

template<typename A, size_t n>
struct CtSizeImpl<std::array<A, n>> {
    using Type = Size<n>;
};

template<typename A, size_t n>
struct CtCapacityImpl<std::array<A, n>> {
    using Type = Size<n>;
};

template<typename A, size_t n>
constexpr auto length(const std::array<A, n>& as) -> Size<n> {
    return Size<n> {};
}

template<typename A, size_t n>
constexpr auto nth(size_t i, const std::array<A, n>& as) -> const A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto nth(size_t i, std::array<A, n>& as) -> A& {
    return as[i];
}

template<typename A, size_t n>
constexpr auto data(const std::array<A, n>& as) -> const A* {
    return as.data();
}

template<typename A, size_t n>
constexpr auto data(std::array<A, n>& as) -> A* {
    return as.data();
}

// Sequence trait implementation for std::vector
template<typename A>
struct ElementImpl<std::vector<A>> {
    using Type = A;
};

template<typename A>
struct CtSizeImpl<std::vector<A>> {
    using Type = Size<dyn>;
};

template<typename A>
struct CtCapacityImpl<std::vector<A>> {
    using Type = Size<dyn>;
};

template<typename A>
constexpr auto length(const std::vector<A>& as) -> size_t {
    return as.size();
}

template<typename A>
constexpr auto nth(size_t i, const std::vector<A>& as) -> const A& {
    return as[i];
}

template<typename A>
constexpr auto nth(size_t i, std::vector<A>& as) -> A& {
    return as[i];
}

template<typename A>
constexpr auto data(const std::vector<A>& as) -> const A* {
    return as.data();
}

template<typename A>
constexpr auto data(std::vector<A>& as) -> A* {
    return as.data();
}

// Sequence trait implementation for std::basic_string
template<typename A>
struct ElementImpl<std::basic_string<A>> {
    using Type = A;
};

template<typename A>
struct CtSizeImpl<std::basic_string<A>> {
    using Type = Size<dyn>;
};

template<typename A>
struct CtCapacityImpl<std::basic_string<A>> {
    using Type = Size<dyn>;
};

template<typename A>
constexpr auto length(const std::basic_string<A>& as) -> size_t {
    return as.size();
}

template<typename A>
constexpr auto nth(size_t i, const std::basic_string<A>& as) -> const A& {
    return as[i];
}

template<typename A>
constexpr auto nth(size_t i, std::basic_string<A>& as) -> A& {
    return as[i];
}

template<typename A>
constexpr auto data(const std::basic_string<A>& as) -> const A* {
    return as.data();
}

template<typename A>
constexpr auto data(std::basic_string<A>& as) -> A* {
    return as.data();
}

};  // namespace efp

#endif