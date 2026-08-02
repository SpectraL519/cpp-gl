#pragma once

#include "sgl/traits.hpp"
#include "sgl/types.hpp"

#include <algorithm>
#include <concepts>
#include <ranges>
#include <span>
#include <vector>

namespace sgl {

template <traits::c_id_type IdType = default_id_type>
struct binary_edge_key {
    using id_type = IdType;
    using key_type = homogeneous_pair<id_type>;

    [[nodiscard]] static constexpr key_type make(const id_type u, const id_type v) noexcept {
        return (u <= v) ? key_type{u, v} : key_type{v, u};
    }

    [[nodiscard]] static constexpr key_type make_directed(const id_type u, const id_type v) noexcept {
        return make(u, v);
    }
};

template <
    traits::c_id_type IdType = default_id_type,
    std::unsigned_integral OccurrenceType = IdType>
struct multi_edge_key_value {
    using id_type = IdType;
    using occurrence_type = OccurrenceType;

    id_type first{};
    id_type second{};
    occurrence_type occurrence{};

    [[nodiscard]] constexpr bool operator==(const multi_edge_key_value&) const noexcept = default;
};

template <
    traits::c_id_type IdType = default_id_type,
    std::unsigned_integral OccurrenceType = IdType>
struct multi_edge_key {
    using id_type = IdType;
    using occurrence_type = OccurrenceType;
    using key_type = multi_edge_key_value<IdType, OccurrenceType>;

    [[nodiscard]] static constexpr key_type make(
        const id_type u, const id_type v, const occurrence_type occurrence
    ) noexcept {
        const auto [a, b] = binary_edge_key<id_type>::make(u, v);
        return key_type{a, b, occurrence};
    }
};

template <traits::c_id_type IdType = default_id_type, typename LabelType = IdType>
struct labeled_edge_key_value {
    using id_type = IdType;
    using label_type = LabelType;

    id_type first{};
    id_type second{};
    label_type label{};

    [[nodiscard]] constexpr bool operator==(const labeled_edge_key_value&) const noexcept = default;
};

template <traits::c_id_type IdType = default_id_type, typename LabelType = IdType>
struct labeled_edge_key {
    using id_type = IdType;
    using label_type = LabelType;
    using key_type = labeled_edge_key_value<IdType, LabelType>;

    [[nodiscard]] static constexpr key_type make(
        const id_type u, const id_type v, const label_type& label
    ) noexcept {
        const auto [a, b] = binary_edge_key<id_type>::make(u, v);
        return key_type{a, b, label};
    }
};

template <traits::c_id_type IdType = default_id_type>
struct hyper_edge_key {
    using id_type = IdType;
    using key_type = std::vector<id_type>;

    [[nodiscard]] static key_type make(std::span<const id_type> endpoints) {
        key_type key(endpoints.begin(), endpoints.end());
        std::ranges::sort(key);
        key.erase(std::ranges::unique(key).begin(), key.end());
        return key;
    }

    template <traits::c_sized_range_of<id_type> R>
    [[nodiscard]] static key_type make(const R& endpoints) {
        key_type key;
        key.reserve(std::ranges::size(endpoints));
        for (const auto id : endpoints)
            key.push_back(id);
        std::ranges::sort(key);
        key.erase(std::ranges::unique(key).begin(), key.end());
        return key;
    }
};

namespace traits {

template <typename KeyPolicy>
concept c_edge_key_policy = requires {
    typename KeyPolicy::id_type;
    typename KeyPolicy::key_type;
    requires c_id_type<typename KeyPolicy::id_type>;
};

template <typename KeyPolicy>
concept c_binary_edge_key_policy =
    c_edge_key_policy<KeyPolicy> and c_instantiation_of<KeyPolicy, binary_edge_key>;

template <typename KeyPolicy>
concept c_multi_edge_key_policy =
    c_edge_key_policy<KeyPolicy> and c_instantiation_of<KeyPolicy, multi_edge_key>;

template <typename KeyPolicy>
concept c_labeled_edge_key_policy =
    c_edge_key_policy<KeyPolicy> and c_instantiation_of<KeyPolicy, labeled_edge_key>;

template <typename KeyPolicy>
concept c_hyper_edge_key_policy =
    c_edge_key_policy<KeyPolicy> and c_instantiation_of<KeyPolicy, hyper_edge_key>;

} // namespace traits

} // namespace sgl
