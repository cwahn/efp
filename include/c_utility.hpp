#ifndef C_UTILITY_HPP_
#define C_UTILITY_HPP_

#include "prelude.hpp"

namespace efp
{
    // FunctionPointerTypeImpl

    template <typename, typename...>
    struct FunctionPointerTypeImpl
    {
    };

    template <typename F, typename... Args>
    struct FunctionPointerTypeImpl<F, Tuple<Args...>>
    {
        using type = Return<F> (*)(Args...);
    };

    // FunctionPointerType

    template <typename F>
    using FunctionPointerType = typename FunctionPointerTypeImpl<F, Arguments<F>>::type;

    // LambdaPointer

    template <typename F>
    struct LambdaPointer
    {
        template <typename Tpl>
        struct Helper
        {
        };

        template <typename... Args>
        struct Helper<Tuple<Args...>>
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

    // func_ptr

    template <typename F>
    // ! Take caution on the lifetime of the argument.
    static FunctionPointerType<F> func_ptr(F &f)
    {
        LambdaPointer<F>::inner_ptr = (void *)&f;
        return (FunctionPointerType<F>)LambdaPointer<F>::template Helper<Arguments<F>>::call;
    }
}

#endif