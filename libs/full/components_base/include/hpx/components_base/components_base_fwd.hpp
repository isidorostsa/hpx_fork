//  Copyright (c) 2007-2024 Hartmut Kaiser
//  Copyright (c) 2011      Bryce Lelbach
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/config.hpp>
#include <hpx/components_base/traits/managed_component_policies.hpp>

namespace hpx::components {

    ///////////////////////////////////////////////////////////////////////
    class pinned_ptr;

    ///////////////////////////////////////////////////////////////////////
    template <typename Component>
    class fixed_component;

    template <typename Component>
    class component;

    template <typename Component, typename Derived = void>
    class managed_component;

    ///////////////////////////////////////////////////////////////////////
    template <typename Component = void>
    class component_base;

    template <typename Component = void>
    class fixed_component_base;

    template <typename Component = void>
    class abstract_component_base;

    template <typename Component>
    using abstract_simple_component_base HPX_DEPRECATED_V(1, 8,
        "The type hpx::components::abstract_simple_component_base is "
        "deprecated. Please use hpx::components::abstract_component_base "
        "instead.") = abstract_component_base<Component>;

    template <typename Component, typename Derived = void>
    class abstract_managed_component_base;

    template <typename Component, typename Wrapper = void,
        typename CtorPolicy = traits::construct_without_back_ptr,
        typename DtorPolicy = traits::managed_object_controls_lifetime>
    class managed_component_base;
}    // namespace hpx::components
