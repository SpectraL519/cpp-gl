#pragma once

#include "sgl/edge_key.hpp"
#include "sgl/traits.hpp"
#include "sgl/types.hpp"

#include <limits>
#include <type_traits>
#include <vector>

namespace sgl {

namespace detail {

template <traits::c_hash_word_type HashWord>
[[nodiscard]] constexpr HashWord splitmix(HashWord x) noexcept {
    x += static_cast<HashWord>(0x9e3779b97f4a7c15ULL);
    x = (x ^ (x >> 30)) * static_cast<HashWord>(0xbf58476d1ce4e5b9ULL);
    x = (x ^ (x >> 27)) * static_cast<HashWord>(0x94d049bb133111ebULL);
    return x ^ (x >> 31);
}

template <traits::c_hash_word_type HashWord>
[[nodiscard]] constexpr HashWord mix(HashWord a, HashWord b) noexcept {
    return splitmix<HashWord>(
        a + static_cast<HashWord>(0x9e3779b97f4a7c15ULL) + (b << 6) + (b >> 2)
    );
}

template <traits::c_hash_word_type HashWord, traits::c_uniform_type UniformType>
[[nodiscard]] constexpr UniformType hash_to_unit_interval(HashWord h) noexcept {
    constexpr auto mantissa_bits = static_cast<HashWord>(
        std::numeric_limits<UniformType>::digits > 0 ? std::numeric_limits<UniformType>::digits : 53
    );
    constexpr auto shift = static_cast<HashWord>(sizeof(HashWord) * 8) > mantissa_bits
                             ? static_cast<HashWord>(sizeof(HashWord) * 8) - mantissa_bits
                             : HashWord{0};
    constexpr auto scale =
        UniformType{1} / static_cast<UniformType>(HashWord{1} << mantissa_bits);
    return static_cast<UniformType>((h >> shift) + HashWord{1}) * scale;
}

template <traits::c_hash_word_type HashWord, traits::c_id_type IdType>
[[nodiscard]] constexpr HashWord hash_id(IdType id) noexcept {
    return splitmix<HashWord>(static_cast<HashWord>(id));
}

template <traits::c_hash_word_type HashWord, typename T>
[[nodiscard]] constexpr HashWord hash_value(const T& value) noexcept {
    if constexpr (std::unsigned_integral<T>) {
        return splitmix<HashWord>(static_cast<HashWord>(value));
    }
    else if constexpr (std::signed_integral<T>) {
        return splitmix<HashWord>(static_cast<HashWord>(value));
    }
    else {
        HashWord h{};
        const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
        for (size_type i = 0uz; i < sizeof(T); ++i)
            h = mix<HashWord>(h, static_cast<HashWord>(bytes[i]));
        return h;
    }
}

} // namespace detail

template <
    traits::c_hash_word_type HashWord = default_hash_word_type,
    traits::c_uniform_type UniformType = default_uniform_type>
struct basic_edge_hash {
    using hash_word_type = HashWord;
    using seed_type = HashWord;
    using uniform_type = UniformType;

    template <traits::c_id_type IdType>
    [[nodiscard]] static constexpr seed_type seed(const homogeneous_pair<IdType>& key) noexcept {
        return detail::mix<hash_word_type>(
            detail::hash_id<hash_word_type>(key.first),
            detail::hash_id<hash_word_type>(key.second)
        );
    }

    template <traits::c_id_type IdType>
    [[nodiscard]] static constexpr uniform_type uniform(
        const homogeneous_pair<IdType>& key, const size_type k
    ) noexcept {
        return detail::hash_to_unit_interval<hash_word_type, uniform_type>(
            detail::mix<hash_word_type>(seed(key), static_cast<hash_word_type>(k))
        );
    }

    template <traits::c_id_type IdType, std::unsigned_integral Occ>
    [[nodiscard]] static constexpr seed_type seed(
        const multi_edge_key_value<IdType, Occ>& key
    ) noexcept {
        return detail::mix<hash_word_type>(
            seed(homogeneous_pair<IdType>{key.first, key.second}),
            detail::hash_value<hash_word_type>(key.occurrence)
        );
    }

    template <traits::c_id_type IdType, std::unsigned_integral Occ>
    [[nodiscard]] static constexpr uniform_type uniform(
        const multi_edge_key_value<IdType, Occ>& key, const size_type k
    ) noexcept {
        return detail::hash_to_unit_interval<hash_word_type, uniform_type>(
            detail::mix<hash_word_type>(seed(key), static_cast<hash_word_type>(k))
        );
    }

    template <traits::c_id_type IdType, typename LabelType>
    [[nodiscard]] static seed_type seed(
        const labeled_edge_key_value<IdType, LabelType>& key
    ) noexcept {
        return detail::mix<hash_word_type>(
            seed(homogeneous_pair<IdType>{key.first, key.second}),
            detail::hash_value<hash_word_type>(key.label)
        );
    }

    template <traits::c_id_type IdType, typename LabelType>
    [[nodiscard]] static uniform_type uniform(
        const labeled_edge_key_value<IdType, LabelType>& key, const size_type k
    ) noexcept {
        return detail::hash_to_unit_interval<hash_word_type, uniform_type>(
            detail::mix<hash_word_type>(seed(key), static_cast<hash_word_type>(k))
        );
    }

    template <traits::c_id_type IdType>
    [[nodiscard]] static seed_type seed(const std::vector<IdType>& key) noexcept {
        seed_type h = static_cast<seed_type>(0x9e3779b97f4a7c15ULL);
        for (const auto id : key)
            h = detail::mix<hash_word_type>(h, detail::hash_id<hash_word_type>(id));
        return h;
    }

    template <traits::c_id_type IdType>
    [[nodiscard]] static uniform_type uniform(
        const std::vector<IdType>& key, const size_type k
    ) noexcept {
        return detail::hash_to_unit_interval<hash_word_type, uniform_type>(
            detail::mix<hash_word_type>(seed(key), static_cast<hash_word_type>(k))
        );
    }
};

using default_edge_hash = basic_edge_hash<>;

namespace traits {

template <typename HashPolicy>
concept c_edge_hash_policy = requires(
    const homogeneous_pair<default_id_type>& key, size_type k
) {
    typename HashPolicy::seed_type;
    typename HashPolicy::uniform_type;
    requires c_hash_word_type<typename HashPolicy::seed_type>;
    requires c_uniform_type<typename HashPolicy::uniform_type>;
    { HashPolicy::seed(key) } -> std::convertible_to<typename HashPolicy::seed_type>;
    { HashPolicy::uniform(key, k) } -> std::convertible_to<typename HashPolicy::uniform_type>;
};

} // namespace traits

} // namespace sgl
