#ifndef EFP_TRAIT_HPP_
#define EFP_TRAIT_HPP_

#include "sfinae.hpp"

namespace efp
{
    IsInvocable<F, Args...>::value && sIsSame<CallReturn<F, Args...>, Ret>::value

} // namespace efp

#endif