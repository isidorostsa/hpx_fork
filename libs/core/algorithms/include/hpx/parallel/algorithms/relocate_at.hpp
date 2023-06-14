//  Copyright (c) 2023 Isidoros Tsaousis-Seiras
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/algorithms/traits/is_trivially_relocatable.hpp>
#include <hpx/type_support/construct_at.hpp>

#include <cstring>
#include <type_traits>

namespace hpx {

#if defined(HPX_HAVE_P1144_STD_RELOCATE_AT)
    using std::relocate_at;    // what would be the include for this?
#else
    template <class T>
    struct destroy_guard
    {
        T& t;
        explicit destroy_guard(T& t)
          : t(t)
        {
        }
// if c++ 20 is available, we can use the destructor
        ~destroy_guard()
        {
            t.~T();
        }
    };

    template <class St, class Dt,
        std::enable_if_t<std::is_same_v<std::decay_t<St>, std::decay_t<Dt>> &&
                hpx::is_trivially_relocatable_v<std::decay_t<St>> &&
                !std::is_volatile_v<St> && !std::is_volatile_v<Dt>,
            int> = 0>
    Dt* hpx_relocate_at_helper(int, St* src, Dt* dst) noexcept
    {
        std::memmove(dst, src, sizeof(St));
        return std::launder(dst);
    }

    // this is move and destroy
    template <class St, class Dt>
   Dt* hpx_relocate_at_helper(long, St* src, Dt* dst) noexcept(
        // has non-throwing move constructor
        std::is_nothrow_constructible_v<Dt, St&&>)
    {
        destroy_guard<St> g(*src);
        return hpx::construct_at(dst, HPX_MOVE(*src));
    }

    template <typename T>
    T* relocate_at(T* src, T* dst) noexcept(
        std::is_nothrow_move_constructible_v<T>)
    {
        static_assert(
            std::is_move_constructible_v<T> && std::is_destructible_v<T>,
            "construct_at(dst, T(std::move(*source)) must be well-formed");

        return hpx::hpx_relocate_at_helper(0, src, dst);    // The zero is a dummy argument
                                                 // to avoid ambiguity with the
                                                 // SFINAE overloads
    }
#endif    // defined(HPX_HAVE_P1144_STD_RELOCATE_AT)

}    // namespace hpx
