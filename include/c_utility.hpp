#ifndef C_UTILITY_HPP_
#define C_UTILITY_HPP_

#include "prelude.hpp"

namespace efp
{
    // FunctionPointerTypeHelper

    template <typename, typename...>
    struct FunctionPointerTypeHelper
    {
    };

    template <typename F, typename... Args>
    struct FunctionPointerTypeHelper<F, std::tuple<Args...>>
    {
        using type = Return_t<F> (*)(Args...);
    };

    // FunctionPointer_t

    template <typename F>
    using FunctionPointer_t = typename FunctionPointerTypeHelper<F, Argument_t<F>>::type;

    // LambdaPointer

    template <typename F>
    struct LambdaPointer
    {
        template <typename Tpl>
        struct Helper
        {
        };

        template <typename... Args>
        struct Helper<std::tuple<Args...>>
        {
            static Return_t<F> call(Args... args)
            {
                return (Return_t<F>)(*(F *)inner_ptr)(args...);
            }
        };

        static void *inner_ptr;
    };

    template <typename F>
    void *LambdaPointer<F>::inner_ptr = nullptr;

    // to_function_pointer

    template <typename F>
    // ! Take caution on the lifetime of the argument.
    static FunctionPointer_t<F> to_function_pointer(F &f)
    {
        LambdaPointer<F>::inner_ptr = (void *)&f;
        return (FunctionPointer_t<F>)LambdaPointer<F>::template Helper<Argument_t<F>>::call;
    }
}

#endif