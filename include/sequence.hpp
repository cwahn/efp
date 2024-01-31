#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

// ct_capacity is compile time bound of length. It does not mean safety of access.
// However, actual capacity does means the length of memory safe to access.

#include "cpp_core.hpp"
#include "meta.hpp"
#include "trait.hpp"

// todo Move and copy assigment operator sort out

namespace efp {

template <typename A, size_t ct_size>
class Array {
public:
    using Element = A;
    using CtSize = Size<ct_size>;
    using CtCapacity = Size<ct_size>;

    Array() {
        for (size_t i = 0; i < ct_size; ++i) {
            new (&data_[i]) Element();
        }
    }

    Array(const Array& other) {
        for (size_t i = 0; i < ct_size; ++i) {
            new (&data_[i]) Element(other.data_[i]);
        }
    }

    Array(Array&& other) noexcept {
        for (size_t i = 0; i < ct_size; ++i) {
            new (&data_[i]) Element(efp::move(other.data_[i])); // Move-construct each element
        }
    }

    template <typename... Arg>
    Array(const Arg&... args)
        : data_{args...} {
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            for (size_t i = 0; i < ct_size; ++i) {
                data_[i] = other.data_[i]; // Use assignment for each element
            }
        }
        return *this;
    }

    Element& operator[](size_t index) {
        return data_[index];
    }

    const Element& operator[](size_t index) const {
        return data_[index];
    }

    bool operator==(const Array& other) const {
        for (size_t i = 0; i < ct_size; ++i) {
            if (data_[i] != other.data_[i]) {
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
            throw std::runtime_error("Array::reserve: capacity must be less than or equal to ct_size");
        }
    }

    const Element* data() const {
        return data_;
    }

    Element* data() {
        return data_;
    }

    Element* begin() {
        return data_;
    }

    const Element* begin() const {
        return data_;
    }

    Element* end() {
        return data_ + ct_size;
    }

    const Element* end() const {
        return data_ + ct_size;
    }

    bool empty() const {
        return ct_size == 0;
    }

private:
    A data_[ct_size];
};

// template <typename A, size_t ct_size>
// using Array = EnableIf<ct_size != dyn, Sequence<A, ct_size, ct_size>>;

template <typename A, size_t n>
struct ElementImpl<Array<A, n>> {
    using Type = A;
};

template <typename A, size_t n>
struct CtSizeImpl<Array<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n>
struct CtCapacityImpl<Array<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n>
constexpr auto length(const Array<A, n>& as) -> Size<n> {
    return Size<n>{};
}

template <typename A, size_t n>
constexpr auto nth(size_t i, const Array<A, n>& as) -> const A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto nth(size_t i, Array<A, n>& as) -> A& {
    return as[i];
}

// static_assert(IsSame<decltype(nth(0, Array<int, 2>{0, 1, 2})), int &>::value, "Assert nth");
// DebugType<decltype(nth(0, Array<int, 2>{0, 1, 2}))> _;

template <typename A, size_t n>
constexpr auto data(const Array<A, n>& as) -> const A* {
    return as.data();
}

template <typename A, size_t n>
constexpr auto data(Array<A, n>& as) -> A* {
    return as.data();
}

template <typename A, size_t ct_capacity>
class ArrVec {
public:
    using Element = A;
    using CtSize = Size<dyn>;
    using CtCapacity = Size<ct_capacity>;

    ArrVec()
        : _size{0} {}

    ArrVec(const ArrVec& other)
        : _size{other._size} {
        for (size_t i = 0; i < _size; ++i) {
            new (&data_[i]) Element(other.data_[i]);
        }
    }

    ArrVec(ArrVec&& other)
        : _size{other._size} {
        other._size = 0;
        for (size_t i = 0; i < _size; ++i) {
            data_[i] = efp::move(other.data_[i]);
        }
    }

    template <typename... Arg>
    ArrVec(const Arg&... args)
        : data_{args...},
          _size(sizeof...(args)) {
    }

    // Constructor from array
    template <size_t ct_size_, typename = EnableIf<ct_capacity >= ct_size_, void>>
    ArrVec(const ArrVec<Element, ct_size_>& as)
        : _size(ct_size_) {
        for (size_t i = 0; i < ct_size_; ++i) {
            new (&data_[i]) Element(as[i]);
        }
    }

    ~ArrVec() {
        for (size_t i = 0; i < _size; ++i) {
            data_[i].~Element();
        }
    }

    ArrVec& operator=(const ArrVec& other) {
        if (this != &other) {
            resize(other.size());
            for (size_t i = 0; i < _size; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    ArrVec& operator=(ArrVec&& other) noexcept {
        if (this != &other) {
            // Destroy existing elements
            for (size_t i = 0; i < _size; ++i) {
                data_[i].~Element();
            }

            // Move data from the source object
            _size = other._size;
            for (size_t i = 0; i < _size; ++i) {
                new (&data_[i]) Element(efp::move(other.data_[i]));
            }

            // Reset the source object
            other._size = 0;
        }
        return *this;
    }

    Element& operator[](size_t index) {
        return data_[index];
    }

    const Element& operator[](size_t index) const {
        return data_[index];
    }

    bool operator==(const ArrVec& other) const {
        if (_size != other._size) {
            return false;
        }

        for (size_t i = 0; i < _size; ++i) {
            if (data_[i] != other.data_[i]) {
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
            throw std::runtime_error("ArrVec::resize: length must be less than or equal to ct_capacity");
        }

        _size = length;
    }

    void reserve(size_t capacity) {
        if (capacity > ct_capacity) {
            throw std::runtime_error("ArrVec::reserve: capacity must be less than or equal to ct_capacity");
        }
    }

    void push_back(const Element& value) {
        if (_size >= ct_capacity) {
            throw std::runtime_error("ArrVec::push_back: size must be less than or equal to ct_capacity");
        } else {
            new (&data_[_size]) Element(value);
            ++_size;
        }
    }

    void push_back(Element&& value) {
        if (_size >= ct_capacity) {
            throw std::runtime_error("ArrVec::push_back: size must be less than or equal to ct_capacity");
        } else {
            new (&data_[_size]) Element(efp::move(value));
            ++_size;
        }
    }

    void insert(size_t index, const Element& value) {
        if (index < 0 || index > _size || _size == ct_capacity) {
            throw std::runtime_error("ArrVec::insert: index must be less than or equal to size and size must be less than or equal to ct_capacity");
        }

        for (size_t i = _size; i > index; --i) {
            new (&data_[i]) Element(efp::move(data_[i - 1]));
            data_[i - 1].~Element();
        }

        new (&data_[index]) Element(value);

        ++_size;
    }

    void pop_back() {
        if (_size == 0) {
            std::runtime_error("ArrVec::pop_back: size must be greater than 0");
        }

        data_[_size - 1].~Element();
        --_size;
    }

    void clear() {
        for (size_t i = 0; i < _size; ++i) {
            data_[i].~Element();
        }
        _size = 0;
    }

    void erase(size_t index) {
        if (index < 0 || index >= _size) {
            throw std::runtime_error("ArrVec::erase: index must be less than or equal to size");
        }
        data_[index].~Element();
        for (size_t i = index; i < _size - 1; ++i) {
            new (&data_[i]) Element(efp::move(data_[i + 1]));
            data_[i + 1].~Element();
        }
        --_size;
    }

    const Element* data() const {
        return data_;
    }

    Element* data() {
        return data_;
    }

    Element* begin() {
        return data_;
    }

    const Element* begin() const {
        return data_;
    }

    Element* end() {
        return data_ + _size;
    }

    const Element* end() const {
        return data_ + _size;
    }

    bool empty() const {
        return _size == 0;
    }

private:
    Element data_[ct_capacity];
    size_t _size;
};

template <typename A, size_t n>
struct ElementImpl<ArrVec<A, n>> {
    using Type = A;
};

template <typename A, size_t n>
struct CtSizeImpl<ArrVec<A, n>> {
    using Type = Size<dyn>;
};

template <typename A, size_t n>
struct CtCapacityImpl<ArrVec<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n>
constexpr auto length(const ArrVec<A, n>& as) -> size_t {
    return as.size();
}

template <typename A, size_t n>
constexpr auto nth(size_t i, const ArrVec<A, n>& as) -> const A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto nth(size_t i, ArrVec<A, n>& as) -> A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto data(const ArrVec<A, n>& as) -> const A* {
    return as.data();
}

template <typename A, size_t n>
constexpr auto data(ArrVec<A, n>& as) -> A* {
    return as.data();
}

template <typename A>
class Vector {
public:
    using Element = A;
    using CtSize = Size<dyn>;
    using CtCapacity = Size<dyn>;

    Vector()
        : data_{nullptr}, _size{0}, capacity_{0} {}

    Vector(const Vector& other)
        : data_{nullptr}, _size{0}, capacity_{0} {
        if (other.data_) {
            _size = other._size;
            capacity_ = other.capacity_;
            data_ = new Element[capacity_];

            // memcpy(data_, other.data_, sizeof(A) * _size);
            for (size_t i = 0; i < other._size; ++i) {
                new (&data_[i]) Element(other.data_[i]);
            }
        }
    }

    Vector(Vector&& other)
        : data_{other.data_}, _size{other._size}, capacity_{other.capacity_} {
        other.data_ = nullptr;
    }

    template <typename... Args>
    Vector(const Args&... args)
        : data_{new Element[sizeof...(args)]},
          capacity_(sizeof...(args)),
          _size(sizeof...(args)) {
        size_t i = 0;
        for (auto arg : std::initializer_list<Common<Args...>>{args...})
            data_[i++] = arg;
    }

    // Constructor from Array
    template <size_t ct_size_>
    Vector(const Array<Element, ct_size_>& as)
        : data_(new Element[ct_size_]),
          _size(ct_size_),
          capacity_(ct_size_) {
        for (size_t i = 0; i < _size; ++i) {
            new (&data_[i]) Element(as[i]);
        }
    }

    // Constructor from ArrVec
    template <size_t ct_cap_>
    Vector(const ArrVec<Element, ct_cap_>& as)
        : data_(new Element[length(as)]),
          _size(length(as)),
          capacity_(ct_cap_) {
        for (size_t i = 0; i < _size; ++i) {
            new (&data_[i]) Element(as[i]);
        }
    }

    ~Vector() {
        if (data_) {
            delete[] data_;
            data_ = nullptr;
        }
    }

    // todo copy_and_swap
    Vector& operator=(const Vector& other) noexcept {
        if (this != &other) {
            Element* newData = nullptr;
            if (other._size > 0) {
                newData = new Element[other.capacity_];

                for (size_t i = 0; i < other._size; ++i) {
                    newData[i] = other.data_[i];
                }
            }

            data_ = newData;
            _size = other._size;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;

            data_ = other.data_;
            _size = other._size;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other._size = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    Element& operator[](size_t index) {
        return data_[index];
    }

    const Element& operator[](size_t index) const {
        return data_[index];
    }

    bool operator==(const Vector& other) const {
        if (_size != other._size) {
            return false;
        }

        for (size_t i = 0; i < _size; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }

        return true;
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return capacity_;
    }

    void resize(size_t length) {
        if (length < 0) {
            throw std::runtime_error("Vector::resize: length must be greater than or equal to 0");
        }

        if (length > capacity_) {
            reserve(length);
        }

        _size = length;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            Element* new_data = new Element[new_capacity];

            for (size_t i = 0; i < _size; ++i) {
                new (&new_data[i]) Element(efp::move(data_[i]));
                data_[i].~Element();
            }

            delete[] data_;

            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    void push_back(const Element& value) {
        if (_size >= capacity_) {
            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
        }

        new (&data_[_size]) Element(value);
        ++_size;
    }

    void push_back(Element&& value) {
        if (_size >= capacity_) {
            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
        }

        new (&data_[_size]) Element(efp::move(value));
        ++_size;
    }

    void insert(size_t index, const Element& value) {
        if (index < 0 || index > _size) {
            throw std::runtime_error("Vector::insert: index must be less than or equal to size");
        }
        if (_size >= capacity_) {
            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
        }
        for (size_t i = _size; i > index; --i) {
            data_[i] = efp::move(data_[i - 1]);
        }
        new (&data_[index]) Element(value);
        ++_size;
    }

    void erase(size_t index) {
        if (index < 0 || index >= _size) {
            throw std::runtime_error("Vector::erase: index must be less than or equal to size");
        }

        data_[index].~Element();
        for (size_t i = index; i < _size - 1; ++i) {
            new (&data_[i]) Element(efp::move(data_[i + 1]));
            data_[i + 1].~Element();
        }

        --_size;
    }

    void clear() {
        for (size_t i = 0; i < _size; ++i) {
            data_[i].~Element();
        }
        _size = 0;
    }

    void pop_back() {
        if (_size == 0) {
            throw std::runtime_error("Vector::pop_back: size must be greater than 0");
        }
        data_[_size - 1].~Element();
        --_size;
    }

    const Element* data() const {
        return data_;
    }

    Element* data() {
        return data_;
    }

    Element* begin() {
        return data_;
    }

    const Element* begin() const {
        return data_;
    }

    Element* end() {
        return data_ + _size;
    }

    const Element* end() const {
        return data_ + _size;
    }

    bool empty() const {
        return _size == 0;
    }

private:
    Element* data_;
    size_t _size;
    size_t capacity_;
};

template <typename A>
struct ElementImpl<Vector<A>> {
    using Type = A;
};

template <typename A>
struct CtSizeImpl<Vector<A>> {
    using Type = Size<dyn>;
};

template <typename A>
struct CtCapacityImpl<Vector<A>> {
    using Type = Size<dyn>;
};

template <typename A>
constexpr auto length(const Vector<A>& as) -> size_t {
    return as.size();
}

template <typename A>
constexpr auto nth(size_t i, const Vector<A>& as) -> const A& {
    return as[i];
}

template <typename A>
constexpr auto nth(size_t i, Vector<A>& as) -> A& {
    return as[i];
}

template <typename A>
constexpr auto data(const Vector<A>& as) -> const A* {
    return as.data();
}

template <typename A>
constexpr auto data(Vector<A>& as) -> A* {
    return as.data();
}

template <typename A, size_t ct_size>
class ArrayView {
public:
    using Element = A;
    using CtSize = Size<ct_size>;
    using CtCapacity = Size<ct_size>;

    ArrayView()
        : data_(nullptr) {
    }

    // ! length will not be used
    ArrayView(Element* data, size_t length = Size<ct_size>{})
        : data_(data) {
        // Ensure that data is not nullptr for a non-empty view.
        if (ct_size > 0 && data_ == nullptr) {
            throw std::runtime_error("ArrayView::ArrayView: data must not be nullptr for a non-empty view");
        }
    }

    Element& operator[](size_t index) {
        return data_[index];
    }

    const Element& operator[](size_t index) const {
        return data_[index];
    }

    bool operator==(const ArrayView& other) const {
        return data_ == other.data_;
    }

    size_t size() const {
        return ct_size;
    }

    size_t capacity() const {
        return ct_size;
    }

    void resize(size_t length) {
        if (length > ct_size || length < 0) {
            throw std::runtime_error("ArrayView::resize: length must be less than or equal to ct_size and greater than or equal to 0");
        }
    }

    void reserve(size_t capacity) {
        if (capacity > ct_size) {
            throw std::runtime_error("ArrayView::reserve: capacity must be less than or equal to ct_size");
        }
    }

    const Element* data() const {
        return data_;
    }

    Element* data() {
        return data_;
    }

    Element* begin() {
        return data_;
    }

    const Element* begin() const {
        return data_;
    }

    Element* end() {
        return data_ + ct_size;
    }

    const Element* end() const {
        return data_ + ct_size;
    }

    bool empty() const {
        return ct_size == 0;
    }

private:
    Element* data_;
};

template <typename A, size_t n>
struct ElementImpl<ArrayView<A, n>> {
    using Type = A;
};

template <typename A, size_t n>
struct CtSizeImpl<ArrayView<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n>
struct CtCapacityImpl<ArrayView<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n>
constexpr auto length(const ArrayView<A, n>& as) -> Size<n> {
    return Size<n>{};
}

template <typename A, size_t n>
constexpr auto nth(size_t i, const ArrayView<A, n>& as) -> const A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto nth(size_t i, ArrayView<A, n>& as) -> A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto data(const ArrayView<A, n>& as) -> const A* {
    return as.data();
}

template <typename A, size_t n>
constexpr auto data(ArrayView<A, n>& as) -> A* {
    return as.data();
}

template <typename A, size_t ct_capacity>
class ArrVecView {
public:
    using Element = A;
    using CtSize = Size<dyn>;
    using CtCapacity = Size<ct_capacity>;

    ArrVecView()
        : data_(nullptr), _size(0) {
    }

    ArrVecView(Element* data, size_t size)
        : data_(data), _size(size) {
        // Ensure that data is not nullptr for a non-empty view.
        if (size > 0 && data_ == nullptr) {
            throw std::runtime_error("ArrVecView::ArrVecView: data must not be nullptr for a non-empty view");
        }
    }

    // Constructor from ArrayView
    template <size_t ct_size_>
    ArrVecView(const ArrayView<Element, ct_size_>& as)
        : data_(as.data()), _size(length(as)) {
    }

    ArrVecView& operator=(const ArrVecView& other) {
        if (this != &other) {
            data_ = other.data_;
            _size = other._size;
        }
        return *this;
    }

    Element& operator[](size_t index) {
        return data_[index];
    }

    const Element& operator[](size_t index) const {
        return data_[index];
    }

    bool operator==(const ArrVecView& other) const {
        return (data_ == other.data_) &&
               (_size == other._size);
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return ct_capacity;
    }

    const Element* data() const {
        return data_;
    }

    Element* data() {
        return data_;
    }

    Element* begin() {
        return data_;
    }

    const Element* begin() const {
        return data_;
    }

    Element* end() {
        return data_ + _size;
    }

    const Element* end() const {
        return data_ + _size;
    }

    bool empty() const {
        return _size == 0;
    }

private:
    Element* data_;
    size_t _size;
};

template <typename A, size_t n>
struct ElementImpl<ArrVecView<A, n>> {
    using Type = A;
};

template <typename A, size_t n>
struct CtSizeImpl<ArrVecView<A, n>> {
    using Type = Size<dyn>;
};

template <typename A, size_t n>
struct CtCapacityImpl<ArrVecView<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n>
constexpr auto length(const ArrVecView<A, n>& as) -> size_t {
    return as.size();
}

template <typename A, size_t n>
constexpr auto nth(size_t i, const ArrVecView<A, n>& as) -> const A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto nth(size_t i, ArrVecView<A, n>& as) -> A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto data(const ArrVecView<A, n>& as) -> const A* {
    return as.data();
}

template <typename A, size_t n>
constexpr auto data(ArrVecView<A, n>& as) -> A* {
    return as.data();
}

template <typename A>
class VectorView {
public:
    using Element = A;
    using CtSize = Size<dyn>;
    using CtCapacity = Size<dyn>;

    VectorView()
        : data_{nullptr}, _size{0}, capacity_{0} {
    }

    VectorView(Element* data, size_t size)
        : data_(data), _size(size), capacity_(size) {
        // Ensure that data is not nullptr for a non-empty view.
        if (size > 0 && data_ == nullptr) {
            throw std::runtime_error("VectorView::VectorView: data must not be nullptr for a non-empty view");
        }
    }

    // Constructor from ArrayView
    template <size_t ct_size_>
    VectorView(const ArrayView<Element, ct_size_>& as)
        : data_(as.data()), _size(length(as)) {
    }

    // Constructor from ArrVecView
    template <size_t ct_cap_>
    VectorView(const ArrVecView<Element, ct_cap_>& as)
        : data_(as.data()), _size(length(as)) {
    }

    VectorView& operator=(const VectorView& other) {
        if (this != &other) {
            data_ = other.data_;
            _size = other._size;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    Element& operator[](size_t index) {
        return data_[index];
    }

    const Element& operator[](size_t index) const {
        return data_[index];
    }

    bool operator==(const VectorView& other) const {
        return (data_ == other.data_) &&
               (_size == other._size) &&
               (capacity_ == other.capacity_);
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return capacity_;
    }

    const Element* data() const {
        return data_;
    }

    Element* data() {
        return data_;
    }

    Element* begin() {
        return data_;
    }

    const Element* begin() const {
        return data_;
    }

    Element* end() {
        return data_ + _size;
    }

    const Element* end() const {
        return data_ + _size;
    }

    bool empty() const {
        return _size == 0;
    }

private:
    Element* data_;
    size_t _size;
    size_t capacity_;
};

// template <typename A>
// using VectorView = SequenceView<A, dyn, dyn>;

template <typename A>
struct ElementImpl<VectorView<A>> {
    using Type = A;
};

template <typename A>
struct CtSizeImpl<VectorView<A>> {
    using Type = Size<dyn>;
};

template <typename A>
struct CtCapacityImpl<VectorView<A>> {
    using Type = Size<dyn>;
};

template <typename A>
constexpr auto length(const VectorView<A>& as) -> size_t {
    return as.size();
}

template <typename A>
constexpr auto nth(size_t i, const VectorView<A>& as) -> const A& {
    return as[i];
}

template <typename A>
constexpr auto nth(size_t i, VectorView<A>& as) -> A& {
    return as[i];
}

template <typename A>
constexpr auto data(const VectorView<A>& as) -> const A* {
    return as.data();
}

template <typename A>
constexpr auto data(VectorView<A>& as) -> A* {
    return as.data();
}

// todo STL only

template <typename A>
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

template <typename A, size_t n>
struct ElementImpl<std::array<A, n>> {
    using Type = A;
};

template <typename A, size_t n>
struct CtSizeImpl<std::array<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n>
struct CtCapacityImpl<std::array<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n>
constexpr auto length(const std::array<A, n>& as) -> Size<n> {
    return Size<n>{};
}

template <typename A, size_t n>
constexpr auto nth(size_t i, const std::array<A, n>& as) -> const A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto nth(size_t i, std::array<A, n>& as) -> A& {
    return as[i];
}

template <typename A, size_t n>
constexpr auto data(const std::array<A, n>& as) -> const A* {
    return as.data();
}

template <typename A, size_t n>
constexpr auto data(std::array<A, n>& as) -> A* {
    return as.data();
}

// Sequence trait implementation for std::vector
template <typename A>
struct ElementImpl<std::vector<A>> {
    using Type = A;
};

template <typename A>
struct CtSizeImpl<std::vector<A>> {
    using Type = Size<dyn>;
};

template <typename A>
struct CtCapacityImpl<std::vector<A>> {
    using Type = Size<dyn>;
};

template <typename A>
constexpr auto length(const std::vector<A>& as) -> size_t {
    return as.size();
}

template <typename A>
constexpr auto nth(size_t i, const std::vector<A>& as) -> const A& {
    return as[i];
}

template <typename A>
constexpr auto nth(size_t i, std::vector<A>& as) -> A& {
    return as[i];
}

template <typename A>
constexpr auto data(const std::vector<A>& as) -> const A* {
    return as.data();
}

template <typename A>
constexpr auto data(std::vector<A>& as) -> A* {
    return as.data();
}

// Sequence trait implementation for std::basic_string
template <typename A>
struct ElementImpl<std::basic_string<A>> {
    using Type = A;
};

template <typename A>
struct CtSizeImpl<std::basic_string<A>> {
    using Type = Size<dyn>;
};

template <typename A>
struct CtCapacityImpl<std::basic_string<A>> {
    using Type = Size<dyn>;
};

template <typename A>
constexpr auto length(const std::basic_string<A>& as) -> size_t {
    return as.size();
}

template <typename A>
constexpr auto nth(size_t i, const std::basic_string<A>& as) -> const A& {
    return as[i];
}

template <typename A>
constexpr auto nth(size_t i, std::basic_string<A>& as) -> A& {
    return as[i];
}

template <typename A>
constexpr auto data(const std::basic_string<A>& as) -> const A* {
    return as.data();
}

template <typename A>
constexpr auto data(std::basic_string<A>& as) -> A* {
    return as.data();
}

}; // namespace efp

#endif