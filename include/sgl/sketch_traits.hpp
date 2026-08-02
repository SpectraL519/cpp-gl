#pragma once

#include "sgl/constants.hpp"
#include "sgl/directional_tags.hpp"
#include "sgl/edge_key.hpp"
#include "sgl/hash.hpp"
#include "sgl/types.hpp"

namespace sgl {

template <
    traits::c_sketch_directional_tag DirectionalTag = undirected_t,
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    traits::c_edge_key_policy EdgeKeyPolicy = binary_edge_key<IdType>,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
struct sketch_traits {
    using directional_tag = DirectionalTag;
    using id_type = IdType;
    using weight_type = WeightType;
    using edge_key_policy = EdgeKeyPolicy;
    using hash_policy = HashPolicy;

    using edge_sample_type = edge_sample<id_type>;
    using key_type = typename edge_key_policy::key_type;

    static constexpr bool is_directed = std::same_as<directional_tag, directed_t>;
    static constexpr bool is_undirected = std::same_as<directional_tag, undirected_t>;
};

template <
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    traits::c_edge_key_policy EdgeKeyPolicy = binary_edge_key<IdType>,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using undirected_sketch_traits =
    sketch_traits<undirected_t, IdType, WeightType, EdgeKeyPolicy, HashPolicy>;

template <
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    traits::c_edge_key_policy EdgeKeyPolicy = binary_edge_key<IdType>,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using directed_sketch_traits =
    sketch_traits<directed_t, IdType, WeightType, EdgeKeyPolicy, HashPolicy>;

template <
    traits::c_sketch_directional_tag DirectionalTag = undirected_t,
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    std::unsigned_integral OccurrenceType = IdType,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using multi_sketch_traits = sketch_traits<
    DirectionalTag,
    IdType,
    WeightType,
    multi_edge_key<IdType, OccurrenceType>,
    HashPolicy>;

template <
    traits::c_sketch_directional_tag DirectionalTag = undirected_t,
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    typename LabelType = IdType,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using labeled_sketch_traits = sketch_traits<
    DirectionalTag,
    IdType,
    WeightType,
    labeled_edge_key<IdType, LabelType>,
    HashPolicy>;

template <
    traits::c_sketch_directional_tag DirectionalTag = undirected_t,
    traits::c_id_type IdType = default_id_type,
    traits::c_weight_type WeightType = default_weight_type,
    traits::c_edge_hash_policy HashPolicy = default_edge_hash>
using hyper_sketch_traits = sketch_traits<
    DirectionalTag,
    IdType,
    WeightType,
    hyper_edge_key<IdType>,
    HashPolicy>;

namespace traits {

template <typename TraitsType>
concept c_sketch_traits = c_instantiation_of<TraitsType, sketch_traits>;

template <typename TraitsType>
concept c_directed_sketch_traits =
    c_sketch_traits<TraitsType>
    and std::same_as<typename TraitsType::directional_tag, directed_t>;

template <typename TraitsType>
concept c_undirected_sketch_traits =
    c_sketch_traits<TraitsType>
    and std::same_as<typename TraitsType::directional_tag, undirected_t>;

template <typename TraitsType>
concept c_binary_sketch_traits =
    c_sketch_traits<TraitsType>
    and c_binary_edge_key_policy<typename TraitsType::edge_key_policy>;

template <typename TraitsType>
concept c_multi_sketch_traits =
    c_sketch_traits<TraitsType>
    and c_multi_edge_key_policy<typename TraitsType::edge_key_policy>;

template <typename TraitsType>
concept c_labeled_sketch_traits =
    c_sketch_traits<TraitsType>
    and c_labeled_edge_key_policy<typename TraitsType::edge_key_policy>;

template <typename TraitsType>
concept c_hyper_sketch_traits =
    c_sketch_traits<TraitsType>
    and c_hyper_edge_key_policy<typename TraitsType::edge_key_policy>;

} // namespace traits

} // namespace sgl
