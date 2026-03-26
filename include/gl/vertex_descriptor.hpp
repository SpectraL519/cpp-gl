// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/decl/graph_traits.hpp"
#include "gl/graph_io.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"
#include "gl/types/properties.hpp"

#include <compare>
#include <format>

namespace gl {

template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
class vertex_descriptor final {
public:
    using type = std::type_identity_t<vertex_descriptor<Properties>>;
    using id_type = IdType;
    using properties_type = Properties;
    using properties_ref_type = std::conditional_t<
        traits::c_empty_properties<properties_type>,
        empty_properties,
        properties_type&>;

    vertex_descriptor() {
        *this = vertex_descriptor::invalid();
    }

    explicit vertex_descriptor(const id_type id)
    requires(traits::c_empty_properties<properties_type>)
    : _id(id) {}

    explicit vertex_descriptor(const id_type id, properties_type& properties)
    requires(traits::c_non_empty_properties<properties_type>)
    : _id(id), _properties(properties) {}

    [[nodiscard]] gl_attr_force_inline static vertex_descriptor invalid() noexcept
    requires(traits::c_empty_properties<properties_type>)
    {
        return vertex_descriptor(constants::invalid_id<id_type>);
    }

    [[nodiscard]] gl_attr_force_inline static vertex_descriptor invalid() noexcept
    requires(traits::c_non_empty_properties<properties_type>)
    {
        static properties_type invalid_properties{};
        return vertex_descriptor(constants::invalid_id<id_type>, invalid_properties);
    }

    vertex_descriptor(const vertex_descriptor&) = default;
    vertex_descriptor& operator=(const vertex_descriptor&) = default;

    vertex_descriptor(vertex_descriptor&&) noexcept = default;
    vertex_descriptor& operator=(vertex_descriptor&&) noexcept = default;

    ~vertex_descriptor() = default;

    [[nodiscard]] gl_attr_force_inline bool operator==(const vertex_descriptor& other
    ) const noexcept {
        return this->_id == other._id;
    }

    [[nodiscard]] gl_attr_force_inline std::strong_ordering operator<=>(
        const vertex_descriptor& other
    ) const noexcept {
        return this->_id <=> other._id;
    }

    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    [[nodiscard]] bool is_valid() const noexcept {
        return this->_id != constants::invalid_id<id_type>;
    }

    [[nodiscard]] gl_attr_force_inline id_type id() const noexcept {
        return this->_id;
    }

    [[nodiscard]] gl_attr_force_inline properties_ref_type properties() const {
        if (not this->is_valid())
            throw std::logic_error("Cannot access properties of an invalid vertex");

        return this->_properties.get();
    }

    friend inline std::ostream& operator<<(std::ostream& os, const vertex_descriptor& vertex) {
        vertex._write(os);
        return os;
    }

private:
    void _write(std::ostream& os) const {
        if constexpr (not traits::c_writable<properties_type>) {
            this->_write_no_properties(os);
            return;
        }
        else {
            if (not io::is_option_set(os, io::graph_option::with_vertex_properties)) {
                this->_write_no_properties(os);
                return;
            }

            if (io::is_option_set(os, io::graph_option::verbose)) {
                os << "[id: " << this->_id << " | properties: " << this->_properties.get() << "]";
            }
            else {
                os << "[" << this->_id << " | " << this->_properties.get() << "]";
            }
        }
    }

    void _write_no_properties(std::ostream& os) const {
        if (io::is_option_set(os, io::graph_option::verbose)) {
            os << std::format("[id: {}]", this->_id);
        }
        else {
            os << this->_id;
        }
    }

    id_type _id;
    [[no_unique_address]] std::conditional_t<
        traits::c_empty_properties<properties_type>,
        empty_properties,
        std::reference_wrapper<properties_type>> _properties;
};

template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using vertex = vertex_descriptor<Properties, IdType>;

} // namespace gl
