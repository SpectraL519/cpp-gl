// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/types/core.hpp"
#include "gl/vertex_descriptor.hpp"
#include "hgl/constants.hpp"
#include "hgl/io/core.hpp"
#include "hgl/traits.hpp"
#include "hgl/types.hpp"

namespace hgl {

// hypergraph vertex descriptor

using gl::vertex_descriptor;

// hyperedge descriptor

template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
class hyperedge_descriptor final {
public:
    using type = hyperedge_descriptor<Properties>;
    using id_type = IdType;
    using properties_type = Properties;

    hyperedge_descriptor() {
        *this = hyperedge_descriptor::invalid();
    }

    explicit hyperedge_descriptor(const id_type id)
    requires(traits::c_empty_properties<properties_type>)
    : _id(id) {}

    explicit hyperedge_descriptor(const id_type id, properties_type& properties)
    requires(traits::c_non_empty_properties<properties_type>)
    : _id(id), _properties(properties) {}

    [[nodiscard]] gl_attr_force_inline static hyperedge_descriptor invalid() noexcept
    requires(traits::c_empty_properties<properties_type>)
    {
        return hyperedge_descriptor(invalid_id);
    }

    [[nodiscard]] gl_attr_force_inline static hyperedge_descriptor invalid() noexcept
    requires(traits::c_non_empty_properties<properties_type>)
    {
        static properties_type invalid_properties{};
        return hyperedge_descriptor(invalid_id, invalid_properties);
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
        return this->_id != invalid_id;
    }

    [[nodiscard]] gl_attr_force_inline id_type id() const noexcept {
        return this->_id;
    }

    [[nodiscard]] gl_attr_force_inline properties_type& properties() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    [[nodiscard]] gl_attr_force_inline properties_type* operator->() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return &this->_properties.get();
    }

    [[nodiscard]] gl_attr_force_inline properties_type& operator*() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    friend std::ostream& operator<<(std::ostream& os, const hyperedge_descriptor& hyperedge) {
        using enum io::detail::option_bit;

        if (io::is_option_set(os, verbose))
            return hyperedge._verbose_write(os);
        else
            return hyperedge._concise_write(os);
    }

private:
    [[noreturn]] void _throw_invalid_access() const {
        throw std::logic_error("Cannot access properties of an invalid hyperedge");
    }

    gl_attr_force_inline void _validate() const {
        if (not this->is_valid())
            this->_throw_invalid_access();
    }

    std::ostream& _verbose_write(std::ostream& os) const {
        using enum io::detail::option_bit;

        os << "[id: " << this->_id;
        if constexpr (traits::c_writable<properties_type>)
            if (io::is_option_set(os, with_vertex_properties))
                os << " | " << this->_properties.get();
        os << ']';

        return os;
    }

    std::ostream& _concise_write(std::ostream& os) const {
        using enum io::detail::option_bit;

        os << this->_id;
        if constexpr (traits::c_writable<properties_type>)
            if (io::is_option_set(os, with_vertex_properties))
                os << '[' << this->_properties.get() << ']';

        return os;
    }

    id_type _id;
    [[no_unique_address]] std::conditional_t<
        traits::c_empty_properties<properties_type>,
        empty_properties,
        std::reference_wrapper<properties_type>> _properties;
};

} // namespace hgl
