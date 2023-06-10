//  Copyright (c) 2023 Isidoros Tsaousis-Seiras
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/config.hpp>
#include <hpx/algorithms/traits/is_trivially_relocatable.hpp>

#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace hpx {

#if defined(HPX_HAVE_P1144_STD_RELOCATE_AT)
    using std::relocate_at;
#else
    template <class T>
    struct destroy_guard
    {
        T& t;
        explicit destroy_guard(T& t)
          : t(t)
        {
        }
        ~destroy_guard()
        {
            t.~T();
        }
    };

    template <class St, class Dt,
        std::enable_if_t<std::is_same_v<    // possibly unnecessary
                             std::remove_cv_t<std::remove_reference_t<St>>,
                             std::remove_cv_t<std::remove_reference_t<Dt>>> &&
                is_trivially_relocatable_v<    // the important part
                    std::remove_cv_t<std::remove_reference_t<St>>> &&
                !std::is_volatile_v<St> && !std::is_volatile_v<Dt>,
            int> = 0>
    Dt* __hpx_relocate_at2(int, St* src, Dt* dst) _NOEXCEPT
    {
        // Do we have this in HPX?
        ::memmove(dst, src, sizeof(St));
        return std::launder(dst);
    }

    template <class St, class Dt>
    Dt* __hpx_relocate_at2(long, St* src, Dt* dst) noexcept(
        std::is_nothrow_constructible_v<Dt, St&&>)
    // non-throwing move constructor
    {
        destroy_guard<St> g(*src);
        return ::new (static_cast<void*>(dst)) T(HPX_MOVE(*src));
    }

    template <typename T>
    constexpr T* relocate_at(T* src, T* dst) noexcept(
        std::is_nothrow_move_constructible_v<T>)
    {
        static_assert(
            std::is_move_constructible_v<T> && std::is_destructible_v<T>,
            "::new (voidify(*dest)) T(std::move(*source)) must be well-formed");

        return __hpx_relocate_at2(
            0, src, dst);    // The zero is a dummy argument
                             // to avoid ambiguity with the
                             // SFINAE overloads
    }
#endif    // defined(HPX_HAVE_P1144_STD_RELOCATE_AT)

}    // namespace hpx
