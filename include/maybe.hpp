// #ifndef MAYBE_HPP_
// #define MAYBE_HPP_

// template <typename T>
// class Maybe
// {
// public:
//     Maybe() : has_value_(false) {}

//     Maybe(const T &value) : has_value_(true)
//     {
//         new (data_) T(value);
//     }

//     Maybe(T &&value) : has_value_(true)
//     {
//         new (data_) T(std::move(value));
//     }

//     Maybe(const Maybe &other) : has_value_(other.has_value_)
//     {
//         if (has_value_)
//         {
//             new (data_) T(*reinterpret_cast<const T *>(other.data_));
//         }
//     }

//     Maybe(Maybe &&other) : has_value_(other.has_value_)
//     {
//         if (has_value_)
//         {
//             new (data_) T(std::move(*reinterpret_cast<T *>(other.data_)));
//         }
//     }

//     ~Maybe()
//     {
//         destructValue();
//     }

//     Maybe &operator=(const Maybe &other)
//     {
//         if (this != &other)
//         {
//             destructValue();
//             has_value_ = other.has_value_;

//             if (has_value_)
//             {
//                 new (data_) T(*reinterpret_cast<const T *>(other.data_));
//             }
//         }
//         return *this;
//     }

//     Maybe &operator=(Maybe &&other)
//     {
//         if (this != &other)
//         {
//             destructValue();
//             has_value_ = other.has_value_;

//             if (has_value_)
//             {
//                 new (data_) T(std::move(*reinterpret_cast<T *>(other.data_)));
//             }
//         }
//         return *this;
//     }

//     bool has_value() const
//     {
//         return has_value_;
//     }

//     explicit operator bool() const
//     {
//         return has_value_;
//     }

//     T &value()
//     {
//         if (!has_value_)
//         {
//             throw std::runtime_error("Maybe has no value");
//         }
//         return *reinterpret_cast<T *>(data_);
//     }

//     const T &value() const
//     {
//         if (!has_value_)
//         {
//             throw std::runtime_error("Maybe has no value");
//         }
//         return *reinterpret_cast<const T *>(data_);
//     }

//     T value_or(T defaultValue) const
//     {
//         return has_value_ ? *reinterpret_cast<const T *>(data_) : defaultValue;
//     }

// private:
//     alignas(T) uint8_t data_[sizeof(T)];
//     bool has_value_;

//     void destructValue()
//     {
//         if (has_value_)
//         {
//             reinterpret_cast<T *>(storage)->~T();
//             has_value_ = false;
//         }
//     }
// };

// #endif