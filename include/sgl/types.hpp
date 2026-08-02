#pragma once

#include "gl/types/core.hpp"
#include "sgl/constants.hpp"
#include "sgl/traits.hpp"

#include <cstdint>
#include <limits>
#include <span>

namespace sgl {

using size_type = gl::size_type;

using default_id_type = gl::default_id_type;

using gl::to_idx;

using gl::to_diff;

template <typename T>
using homogeneous_pair = gl::homogeneous_pair<T>;

using default_weight_type = double;

using default_occurrence_type = default_id_type;

using default_label_type = default_id_type;

using default_hash_word_type = std::uint64_t;

using default_uniform_type = default_weight_type;

template <traits::c_id_type IdType = default_id_type>
struct edge_sample {
    using id_type = IdType;

    id_type u = invalid_id_v<id_type>;
    id_type v = invalid_id_v<id_type>;

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return this->u != invalid_id_v<id_type> and this->v != invalid_id_v<id_type>;
    }

    [[nodiscard]] constexpr bool operator==(const edge_sample&) const noexcept = default;
};

template <traits::c_weight_type WeightType, traits::c_id_type IdType = default_id_type>
struct vertex_sketch_view {
    using weight_type = WeightType;
    using id_type = IdType;
    using edge_sample_type = edge_sample<id_type>;

    std::span<weight_type> S;
    std::span<edge_sample_type> F;
    weight_type* max_s = nullptr;

    [[nodiscard]] constexpr size_type width() const noexcept {
        return this->S.size();
    }

    [[nodiscard]] constexpr bool valid() const noexcept {
        return this->max_s != nullptr and this->S.size() == this->F.size() and not this->S.empty();
    }
};

template <traits::c_weight_type WeightType, traits::c_id_type IdType = default_id_type>
struct const_vertex_sketch_view {
    using weight_type = WeightType;
    using id_type = IdType;
    using edge_sample_type = edge_sample<id_type>;

    std::span<const weight_type> S;
    std::span<const edge_sample_type> F;
    const weight_type* max_s = nullptr;

    [[nodiscard]] constexpr size_type width() const noexcept {
        return this->S.size();
    }

    [[nodiscard]] constexpr bool valid() const noexcept {
        return this->max_s != nullptr and this->S.size() == this->F.size() and not this->S.empty();
    }
};

} // namespace sgl
