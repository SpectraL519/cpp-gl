// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/vertex_descriptor.hpp"
#include "hgl/constants.hpp"
#include "hgl/traits.hpp"
#include "hgl/types.hpp"

namespace hgl {

// hypergraph vertex descriptor

template <traits::c_properties Properties = types::empty_properties>
using vertex_descriptor = gl::vertex_descriptor<Properties>;

// hyperedge descriptor

template <traits::c_properties Properties = types::empty_properties>
class hyperedge_descriptor final {
public:
    using type = hyperedge_descriptor<Properties>;
    using properties_type = Properties;
    using properties_ref_type = std::conditional_t<
        traits::c_empty_properties<properties_type>,
        types::empty_properties,
        properties_type&>;

    hyperedge_descriptor() {
        *this = hyperedge_descriptor::invalid();
    }

    explicit hyperedge_descriptor(const types::id_type id)
    requires(traits::c_empty_properties<properties_type>)
    : _id(id) {}

    explicit hyperedge_descriptor(const types::id_type id, properties_type& properties)
    requires(traits::c_non_empty_properties<properties_type>)
    : _id(id), _properties(properties) {}

    [[nodiscard]] gl_attr_force_inline static hyperedge_descriptor invalid() noexcept
    requires(traits::c_empty_properties<properties_type>)
    {
        return hyperedge_descriptor(constants::invalid_id);
    }

    [[nodiscard]] gl_attr_force_inline static hyperedge_descriptor invalid() noexcept
    requires(traits::c_non_empty_properties<properties_type>)
    {
        static properties_type invalid_properties{};
        return hyperedge_descriptor(constants::invalid_id, invalid_properties);
    }

    hyperedge_descriptor(const hyperedge_descriptor&) = default;
    hyperedge_descriptor& operator=(const hyperedge_descriptor&) = default;

    hyperedge_descriptor(hyperedge_descriptor&&) noexcept = default;
    hyperedge_descriptor& operator=(hyperedge_descriptor&&) noexcept = default;

    ~hyperedge_descriptor() = default;

    [[nodiscard]] bool operator==(const hyperedge_descriptor& other) const noexcept {
        return this->_id == other._id;
    }

    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    [[nodiscard]] gl_attr_force_inline std::strong_ordering operator<=>(
        const hyperedge_descriptor& other
    ) const noexcept {
        return this->_id <=> other._id;
    }

    [[nodiscard]] gl_attr_force_inline bool is_valid() const noexcept {
        return this->_id != constants::invalid_id;
    }

    [[nodiscard]] gl_attr_force_inline types::id_type id() const noexcept {
        return this->_id;
    }

    [[nodiscard]] properties_ref_type properties() const {
        if (not this->is_valid())
            throw std::logic_error("Cannot access properties of an invalid hyperedge");

        return this->_properties.get();
    }

private:
    types::id_type _id;
    [[no_unique_address]] std::conditional_t<
        traits::c_empty_properties<properties_type>,
        types::empty_properties,
        std::reference_wrapper<properties_type>> _properties;
};

template <traits::c_properties Properties = types::empty_properties>
using hyperedge = hyperedge_descriptor<Properties>;

} // namespace hgl
