#ifndef C_UTILITY_HPP_
#define C_UTILITY_HPP_

#include "prelude.hpp"

namespace efp
{
    // FunctionPointerType

    template <typename, typename...>
    struct FunctionPointerType
    {
    };

    template <typename F, typename... Args>
    struct FunctionPointerType<F, std::tuple<Args...>>
    {
        using type = Return<F>(*)(Args...);
    };

    // FunctionPointer_t

    template <typename F>
    using FunctionPointer_t = typename FunctionPointerType<F, Arguments<F>>::type;

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
            static Return<F> call(Args... args)
            {
                return (Return<F>)(*(F *)inner_ptr)(args...);
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
        return (FunctionPointer_t<F>)LambdaPointer<F>::template Helper<Arguments<F>>::call;
    }
}

#endif