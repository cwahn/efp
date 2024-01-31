#ifndef C_UTILITY_HPP_
#define C_UTILITY_HPP_

#include "efp/prelude.hpp"

namespace efp {
// FunctionPosize_terTypeImpl

template <typename, typename...>
struct FunctionPosize_terTypeImpl {
};

template <typename F, typename... Args>
struct FunctionPosize_terTypeImpl<F, Tuple<Args...>> {
    using type = Return<F> (*)(Args...);
};

// FunctionPosize_terType

template <typename F>
using FunctionPosize_terType = typename FunctionPosize_terTypeImpl<F, Arguments<F>>::type;

// LambdaPosize_ter

template <typename F>
struct LambdaPosize_ter {
    template <typename Tpl>
    struct Helper {
    };

    template <typename... Args>
    struct Helper<Tuple<Args...>> {
        static Return<F> call(Args... args) {
            return (Return<F>)(*(F*)inner_ptr)(args...);
        }
    };

    static void* inner_ptr;
};

template <typename F>
void* LambdaPosize_ter<F>::inner_ptr = nullptr;

// func_ptr

template <typename F>
// ! Take caution on the lifetime of the argument.
static FunctionPosize_terType<F> func_ptr(F& f) {
    LambdaPosize_ter<F>::inner_ptr = (void*)&f;
    return (FunctionPosize_terType<F>)LambdaPosize_ter<F>::template Helper<Arguments<F>>::call;
}
} // namespace efp

#endif