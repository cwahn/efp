#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

// ct_capacity is compile time bound of length. It does not mean safety of access.
// However, actual capacity does means the length of memory safe to access.

#include "cpp_core.hpp"
#include "sfinae.hpp"
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
            abort();
        }
    }

    void reserve(size_t capacity) {
        if (capacity > ct_size) {
            abort();
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
        : size_{0} {}

    ArrVec(const ArrVec& other)
        : size_{other.size_} {
        for (size_t i = 0; i < size_; ++i) {
            new (&data_[i]) Element(other.data_[i]);
        }
    }

    ArrVec(ArrVec&& other)
        : size_{other.size_} {
        other.size_ = 0;
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = efp::move(other.data_[i]);
        }
    }

    template <typename... Arg>
    ArrVec(const Arg&... args)
        : data_{args...},
          size_(sizeof...(args)) {
    }

    // Constructor from array
    template <size_t ct_size_, typename = EnableIf<ct_capacity >= ct_size_, void>>
    ArrVec(const ArrVec<Element, ct_size_>& as)
        : size_(ct_size_) {
        for (size_t i = 0; i < ct_size_; ++i) {
            new (&data_[i]) Element(as[i]);
        }
    }

    ~ArrVec() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~Element();
        }
    }

    ArrVec& operator=(const ArrVec& other) {
        if (this != &other) {
            resize(other.size());
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    ArrVec& operator=(ArrVec&& other) noexcept {
        if (this != &other) {
            // Destroy existing elements
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~Element();
            }

            // Move data from the source object
            size_ = other.size_;
            for (size_t i = 0; i < size_; ++i) {
                new (&data_[i]) Element(efp::move(other.data_[i]));
            }

            // Reset the source object
            other.size_ = 0;
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
        if (size_ != other.size_) {
            return false;
        }

        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }

        return true;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return ct_capacity;
    }

    void resize(size_t length) {
        if (length > ct_capacity || length < 0) {
            abort();
        }

        size_ = length;
    }

    void reserve(size_t capacity) {
        if (capacity > ct_capacity) {
            abort();
        }
    }

    void push_back(const Element& value) {
        if (size_ >= ct_capacity) {
            abort();
        } else {
            new (&data_[size_]) Element(value);
            ++size_;
        }
    }

    void push_back(Element&& value) {
        if (size_ >= ct_capacity) {
            abort();
        } else {
            new (&data_[size_]) Element(efp::move(value));
            ++size_;
        }
    }

    void insert(size_t index, const Element& value) {
        if (index < 0 || index > size_ || size_ == ct_capacity) {
            abort();
        }

        for (size_t i = size_; i > index; --i) {
            new (&data_[i]) Element(efp::move(data_[i - 1]));
            data_[i - 1].~Element();
        }

        new (&data_[index]) Element(value);

        ++size_;
    }

    void pop_back() {
        if (size_ == 0) {
            abort();
        }

        data_[size_ - 1].~Element();
        --size_;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~Element();
        }
        size_ = 0;
    }

    void erase(size_t index) {
        if (index < 0 || index >= size_) {
            abort();
        }
        data_[index].~Element();
        for (size_t i = index; i < size_ - 1; ++i) {
            new (&data_[i]) Element(efp::move(data_[i + 1]));
            data_[i + 1].~Element();
        }
        --size_;
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
        return data_ + size_;
    }

    const Element* end() const {
        return data_ + size_;
    }

    bool empty() const {
        return size_ == 0;
    }

private:
    Element data_[ct_capacity];
    size_t size_;
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
        : data_{nullptr}, size_{0}, capacity_{0} {}

    Vector(const Vector& other)
        : data_{nullptr}, size_{0}, capacity_{0} {
        if (other.data_) {
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new Element[capacity_];

            // memcpy(data_, other.data_, sizeof(A) * size_);
            for (size_t i = 0; i < other.size_; ++i) {
                new (&data_[i]) Element(other.data_[i]);
            }
        }
    }

    Vector(Vector&& other)
        : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_} {
        other.data_ = nullptr;
    }

    template <typename... Args>
    Vector(const Args&... args)
        : data_{new Element[sizeof...(args)]},
          capacity_(sizeof...(args)),
          size_(sizeof...(args)) {
        size_t i = 0;
        for (auto arg : std::initializer_list<Common<Args...>>{args...})
            data_[i++] = arg;
    }

    // Constructor from Array
    template <size_t ct_size_>
    Vector(const Array<Element, ct_size_>& as)
        : data_(new Element[ct_size_]),
          size_(ct_size_),
          capacity_(ct_size_) {
        for (size_t i = 0; i < size_; ++i) {
            new (&data_[i]) Element(as[i]);
        }
    }

    // Constructor from ArrVec
    template <size_t ct_cap_>
    Vector(const ArrVec<Element, ct_cap_>& as)
        : data_(new Element[length(as)]),
          size_(length(as)),
          capacity_(ct_cap_) {
        for (size_t i = 0; i < size_; ++i) {
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
            if (other.size_ > 0) {
                newData = new Element[other.capacity_];

                for (size_t i = 0; i < other.size_; ++i) {
                    newData[i] = other.data_[i];
                }
            }

            data_ = newData;
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
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
        if (size_ != other.size_) {
            return false;
        }

        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }

        return true;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    void resize(size_t length) {
        if (length < 0) {
            abort();
        }

        if (length > capacity_) {
            reserve(length);
        }

        size_ = length;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            Element* new_data = new Element[new_capacity];

            for (size_t i = 0; i < size_; ++i) {
                new (&new_data[i]) Element(efp::move(data_[i]));
                data_[i].~Element();
            }

            delete[] data_;

            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    void push_back(const Element& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
        }

        new (&data_[size_]) Element(value);
        ++size_;
    }

    void push_back(Element&& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
        }

        new (&data_[size_]) Element(efp::move(value));
        ++size_;
    }

    void insert(size_t index, const Element& value) {
        if (index < 0 || index > size_) {
            abort();
        }
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
        }
        for (size_t i = size_; i > index; --i) {
            data_[i] = efp::move(data_[i - 1]);
        }
        new (&data_[index]) Element(value);
        ++size_;
    }

    void erase(size_t index) {
        if (index < 0 || index >= size_) {
            abort();
        }

        data_[index].~Element();
        for (size_t i = index; i < size_ - 1; ++i) {
            new (&data_[i]) Element(efp::move(data_[i + 1]));
            data_[i + 1].~Element();
        }

        --size_;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~Element();
        }
        size_ = 0;
    }

    void pop_back() {
        if (size_ == 0) {
            abort();
        }
        data_[size_ - 1].~Element();
        --size_;
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
        return data_ + size_;
    }

    const Element* end() const {
        return data_ + size_;
    }

    bool empty() const {
        return size_ == 0;
    }

private:
    Element* data_;
    size_t size_;
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
            abort();
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
            abort();
        }
    }

    void reserve(size_t capacity) {
        if (capacity > ct_size) {
            abort();
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
        : data_(nullptr), size_(0) {
    }

    ArrVecView(Element* data, size_t size)
        : data_(data), size_(size) {
        // Ensure that data is not nullptr for a non-empty view.
        if (size > 0 && data_ == nullptr) {
            abort();
        }
    }

    // Constructor from ArrayView
    template <size_t ct_size_>
    ArrVecView(const ArrayView<Element, ct_size_>& as)
        : data_(as.data()), size_(length(as)) {
    }

    ArrVecView& operator=(const ArrVecView& other) {
        if (this != &other) {
            data_ = other.data_;
            size_ = other.size_;
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
               (size_ == other.size_);
    }

    size_t size() const {
        return size_;
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
        return data_ + size_;
    }

    const Element* end() const {
        return data_ + size_;
    }

    bool empty() const {
        return size_ == 0;
    }

private:
    Element* data_;
    size_t size_;
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
        : data_{nullptr}, size_{0}, capacity_{0} {
    }

    VectorView(Element* data, size_t size)
        : data_(data), size_(size), capacity_(size) {
        // Ensure that data is not nullptr for a non-empty view.
        if (size > 0 && data_ == nullptr) {
            abort();
        }
    }

    // Constructor from ArrayView
    template <size_t ct_size_>
    VectorView(const ArrayView<Element, ct_size_>& as)
        : data_(as.data()), size_(length(as)) {
    }

    // Constructor from ArrVecView
    template <size_t ct_cap_>
    VectorView(const ArrVecView<Element, ct_cap_>& as)
        : data_(as.data()), size_(length(as)) {
    }

    VectorView& operator=(const VectorView& other) {
        if (this != &other) {
            data_ = other.data_;
            size_ = other.size_;
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
               (size_ == other.size_) &&
               (capacity_ == other.capacity_);
    }

    size_t size() const {
        return size_;
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
        return data_ + size_;
    }

    const Element* end() const {
        return data_ + size_;
    }

    bool empty() const {
        return size_ == 0;
    }

private:
    Element* data_;
    size_t size_;
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