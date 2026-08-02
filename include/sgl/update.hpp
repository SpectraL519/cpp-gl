#pragma once

#include "sgl/sketch_traits.hpp"
#include "sgl/types.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace sgl {
namespace detail {

template <traits::c_hash_word_type SeedType>
class fisher_yates_perm {
public:
    using seed_type = SeedType;

    explicit fisher_yates_perm(const size_type m, const seed_type seed)
    : _perm(m)
    , _state(seed == seed_type{} ? static_cast<seed_type>(0x9e3779b97f4a7c15ULL) : seed) {
        for (size_type i = 0uz; i < m; ++i)
            this->_perm[i] = i;
    }

    [[nodiscard]] size_type next_slot(const size_type k) {
        const auto remaining = static_cast<seed_type>(this->_perm.size() - k);
        const auto r = k + static_cast<size_type>(this->_next() % remaining);
        std::swap(this->_perm[k], this->_perm[r]);
        return this->_perm[k];
    }

private:
    [[nodiscard]] seed_type _next() noexcept {
        this->_state ^= this->_state >> 12;
        this->_state ^= this->_state << 25;
        this->_state ^= this->_state >> 27;
        return this->_state * static_cast<seed_type>(0x2545F4914F6CDD1DULL);
    }

    std::vector<size_type> _perm;
    seed_type _state;
};

template <traits::c_sketch_traits Traits, typename Key>
void update_es(
    vertex_sketch_view<typename Traits::weight_type, typename Traits::id_type> row,
    const Key& key,
    const typename Traits::weight_type weight,
    const edge_sample<typename Traits::id_type>& sample_to_store
) {
    using weight_type = typename Traits::weight_type;
    using hash_policy = typename Traits::hash_policy;
    using seed_type = typename hash_policy::seed_type;

    if (not row.valid())
        throw std::invalid_argument("Invalid vertex sketch view passed to update_es");
    if (weight <= static_cast<weight_type>(0))
        throw std::invalid_argument("EdgeSketch edge weight must be positive");

    const auto m = row.width();
    fisher_yates_perm<seed_type> perm(m, hash_policy::seed(key));

    weight_type sum = static_cast<weight_type>(0);
    bool update_max = false;

    for (size_type k = 0uz; k < m; ++k) {
        const auto U = static_cast<weight_type>(hash_policy::uniform(key, k + 1uz));
        const auto E = -std::log(U) / weight;
        sum += E / static_cast<weight_type>(m - k);

        if (sum >= *row.max_s)
            break;

        const auto l = perm.next_slot(k);
        if (sum < row.S[l]) {
            if (row.S[l] == *row.max_s)
                update_max = true;
            row.S[l] = sum;
            row.F[l] = sample_to_store;
        }
    }

    if (update_max) {
        weight_type mx = row.S[0];
        for (size_type j = 1uz; j < m; ++j)
            mx = std::max(mx, row.S[j]);
        *row.max_s = mx;
    }
}

} // namespace detail
} // namespace sgl
