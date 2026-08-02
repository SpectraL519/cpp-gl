#pragma once

#include "gl/attributes/force_inline.hpp"
#include "sgl/directional_tags.hpp"
#include "sgl/edge_key.hpp"
#include "sgl/sketch_traits.hpp"
#include "sgl/registry.hpp"
#include "sgl/update.hpp"

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace sgl {

template <traits::c_sketch_traits Traits = sketch_traits<>>
class sketch;

namespace traits {

template <typename S>
concept c_sketch = c_instantiation_of<std::remove_cvref_t<S>, sketch>;

template <typename S>
concept c_directed_sketch =
    c_sketch<S> and c_directed_sketch_traits<typename std::remove_cvref_t<S>::traits_type>;

template <typename S>
concept c_undirected_sketch =
    c_sketch<S> and c_undirected_sketch_traits<typename std::remove_cvref_t<S>::traits_type>;

} // namespace traits

template <traits::c_sketch_traits Traits>
class sketch {
public:
    using traits_type = Traits;
    using directional_tag = typename traits_type::directional_tag;
    using id_type = typename traits_type::id_type;
    using weight_type = typename traits_type::weight_type;
    using edge_key_policy = typename traits_type::edge_key_policy;
    using hash_policy = typename traits_type::hash_policy;
    using edge_sample_type = typename traits_type::edge_sample_type;
    using key_type = typename traits_type::key_type;
    using registry_type = sketch_registry<traits_type>;
    using view_type = typename registry_type::view_type;
    using const_view_type = typename registry_type::const_view_type;

    sketch() : sketch(default_sketch_width) {}

    explicit sketch(const size_type m) : _registry(m) {}

    sketch(sketch&&) noexcept = default;
    sketch& operator=(sketch&&) noexcept = default;
    sketch(const sketch&) = default;
    sketch& operator=(const sketch&) = default;
    ~sketch() = default;

    [[nodiscard]] gl_attr_force_inline size_type width() const noexcept {
        return this->_registry.width();
    }

    [[nodiscard]] gl_attr_force_inline size_type n_vertices() const noexcept {
        return this->_registry.n_vertices();
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const id_type id) const {
        return this->_registry.has_vertex(id);
    }

    [[nodiscard]] gl_attr_force_inline const std::vector<id_type>& vertex_ids() const noexcept {
        return this->_registry.vertex_ids();
    }

    [[nodiscard]] gl_attr_force_inline view_type sketch_of(const id_type id) {
        return this->_registry.view_of(id);
    }

    [[nodiscard]] gl_attr_force_inline const_view_type sketch_of(const id_type id) const {
        return this->_registry.view_of(id);
    }

    [[nodiscard]] gl_attr_force_inline registry_type& registry() noexcept {
        return this->_registry;
    }

    [[nodiscard]] gl_attr_force_inline const registry_type& registry() const noexcept {
        return this->_registry;
    }

    void update_edge(const id_type u, const id_type v, const weight_type weight = weight_type{1})
    requires(traits::c_binary_sketch_traits<traits_type>)
    {
        const auto key = edge_key_policy::make(u, v);
        const edge_sample_type sample{u, v};
        this->_update_edge_binary(u, v, key, weight, sample);
    }

    template <typename Occurrence>
    void update_edge(
        const id_type u,
        const id_type v,
        const Occurrence occurrence,
        const weight_type weight = weight_type{1}
    )
    requires(
        traits::c_multi_sketch_traits<traits_type>
        and std::convertible_to<Occurrence, typename edge_key_policy::occurrence_type>
    )
    {
        const auto key = edge_key_policy::make(
            u, v, static_cast<typename edge_key_policy::occurrence_type>(occurrence)
        );
        const edge_sample_type sample{u, v};
        this->_update_edge_binary(u, v, key, weight, sample);
    }

    template <typename Label>
    void update_edge(
        const id_type u,
        const id_type v,
        const Label& label,
        const weight_type weight = weight_type{1}
    )
    requires(
        traits::c_labeled_sketch_traits<traits_type>
        and std::convertible_to<Label, typename edge_key_policy::label_type>
    )
    {
        const auto key = edge_key_policy::make(u, v, label);
        const edge_sample_type sample{u, v};
        this->_update_edge_binary(u, v, key, weight, sample);
    }

    void update_edge(
        const traits::c_sized_range_of<id_type> auto& endpoints,
        const weight_type weight = weight_type{1}
    )
    requires(traits::c_hyper_sketch_traits<traits_type>)
    {
        const auto key = edge_key_policy::make(endpoints);
        if (key.size() < 1uz)
            throw std::invalid_argument("Hyperedge must contain at least one endpoint");

        const auto anchor = key.front();
        for (const auto id : key) {
            const auto idx = this->_registry.get_or_create(id);
            const edge_sample_type sample{anchor, id};
            detail::update_es<traits_type>(this->_registry.view_at(idx), key, weight, sample);
        }
    }

    void merge(const sketch& other) {
        if (other.width() != this->width())
            throw std::invalid_argument("Cannot merge EdgeSketches with different widths");

        for (const auto id : other.vertex_ids()) {
            const auto dst = this->_registry.get_or_create(id);
            this->_registry.merge_min(dst, other._registry.view_of(id));
        }
    }

    [[nodiscard]] weight_type estimate_degree(const id_type id) const {
        const auto row = this->_registry.view_of(id);
        weight_type sum = static_cast<weight_type>(0);
        for (const auto s : row.S) {
            if (not std::isfinite(s))
                return static_cast<weight_type>(0);
            sum += s;
        }
        if (sum <= static_cast<weight_type>(0))
            return static_cast<weight_type>(0);
        return static_cast<weight_type>(row.width() - 1uz) / sum;
    }

    void reserve(const size_type n_vertices) {
        this->_registry.reserve(n_vertices);
    }

    void clear() {
        this->_registry.clear();
    }

private:
    template <typename Key>
    void _update_edge_binary(
        const id_type u,
        const id_type v,
        const Key& key,
        const weight_type weight,
        const edge_sample_type& sample
    ) {
        const auto update_one = [&](const id_type vertex) {
            const auto idx = this->_registry.get_or_create(vertex);
            detail::update_es<traits_type>(this->_registry.view_at(idx), key, weight, sample);
        };

        if constexpr (traits_type::is_directed) {
            update_one(u);
        }
        else {
            update_one(u);
            if (u != v)
                update_one(v);
        }
    }

    registry_type _registry;
};

template <
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    traits::c_edge_key_policy EdgeKeyPolicy = binary_edge_key<IdType>,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using undirected_sketch =
    sketch<undirected_sketch_traits<IdType, WeightType, EdgeKeyPolicy, HashPolicy>>;

template <
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    traits::c_edge_key_policy EdgeKeyPolicy = binary_edge_key<IdType>,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using directed_sketch =
    sketch<directed_sketch_traits<IdType, WeightType, EdgeKeyPolicy, HashPolicy>>;

template <
    traits::c_sketch_directional_tag DirectionalTag = undirected_t,
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    std::unsigned_integral OccurrenceType = IdType,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using multi_sketch = sketch<
    multi_sketch_traits<DirectionalTag, IdType, WeightType, OccurrenceType, HashPolicy>>;

template <
    traits::c_sketch_directional_tag DirectionalTag = undirected_t,
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    typename LabelType = IdType,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using labeled_sketch = sketch<
    labeled_sketch_traits<DirectionalTag, IdType, WeightType, LabelType, HashPolicy>>;

template <
    traits::c_sketch_directional_tag DirectionalTag = undirected_t,
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using hyper_sketch =
    sketch<hyper_sketch_traits<DirectionalTag, IdType, WeightType, HashPolicy>>;

} // namespace sgl
