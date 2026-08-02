#pragma once

#include "gl/attributes/force_inline.hpp"
#include "sgl/constants.hpp"
#include "sgl/sketch_traits.hpp"
#include "sgl/types.hpp"

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sgl {

template <traits::c_sketch_traits Traits>
class sketch_registry {
public:
    using traits_type = Traits;
    using id_type = typename traits_type::id_type;
    using weight_type = typename traits_type::weight_type;
    using edge_sample_type = typename traits_type::edge_sample_type;

    using view_type = vertex_sketch_view<weight_type, id_type>;
    using const_view_type = const_vertex_sketch_view<weight_type, id_type>;

    sketch_registry() : sketch_registry(default_sketch_width) {}

    explicit sketch_registry(const size_type m)
    : _m(m), _m_stride(align_sketch_stride(m, sizeof(weight_type))) {
        if (m < 2uz)
            throw std::invalid_argument("EdgeSketch width m must be >= 2");
    }

    sketch_registry(const sketch_registry&) = default;
    sketch_registry(sketch_registry&&) noexcept = default;
    sketch_registry& operator=(const sketch_registry&) = default;
    sketch_registry& operator=(sketch_registry&&) noexcept = default;
    ~sketch_registry() = default;

    [[nodiscard]] gl_attr_force_inline size_type width() const noexcept {
        return this->_m;
    }

    [[nodiscard]] gl_attr_force_inline size_type stride() const noexcept {
        return this->_m_stride;
    }

    [[nodiscard]] gl_attr_force_inline size_type n_vertices() const noexcept {
        return this->_inv_ids.size();
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const id_type id) const {
        return this->_id_map.contains(id);
    }

    [[nodiscard]] gl_attr_force_inline const std::vector<id_type>& vertex_ids() const noexcept {
        return this->_inv_ids;
    }

    size_type get_or_create(const id_type id) {
        if (const auto it = this->_id_map.find(id); it != this->_id_map.end())
            return it->second;

        const auto idx = this->_inv_ids.size();
        this->_id_map.emplace(id, idx);
        this->_inv_ids.push_back(id);
        this->_grow_to(idx + 1uz);
        this->_init_row(idx);
        return idx;
    }

    [[nodiscard]] size_type index_of(const id_type id) const {
        const auto it = this->_id_map.find(id);
        if (it == this->_id_map.end())
            throw std::invalid_argument("Vertex id is not present in the sketch registry");
        return it->second;
    }

    [[nodiscard]] gl_attr_force_inline id_type id_at(const size_type idx) const {
        return this->_inv_ids.at(idx);
    }

    [[nodiscard]] view_type view_at(const size_type idx) {
        this->_verify_idx(idx);
        const auto off = idx * this->_m_stride;
        return view_type{
            std::span<weight_type>{this->_s.data() + off, this->_m},
            std::span<edge_sample_type>{this->_f.data() + off, this->_m},
            &this->_max_s[idx]
        };
    }

    [[nodiscard]] const_view_type view_at(const size_type idx) const {
        this->_verify_idx(idx);
        const auto off = idx * this->_m_stride;
        return const_view_type{
            std::span<const weight_type>{this->_s.data() + off, this->_m},
            std::span<const edge_sample_type>{this->_f.data() + off, this->_m},
            &this->_max_s[idx]
        };
    }

    [[nodiscard]] gl_attr_force_inline view_type view_of(const id_type id) {
        return this->view_at(this->index_of(id));
    }

    [[nodiscard]] gl_attr_force_inline const_view_type view_of(const id_type id) const {
        return this->view_at(this->index_of(id));
    }

    void merge_min(const size_type dst_idx, const const_view_type& src) {
        auto dst = this->view_at(dst_idx);
        if (src.width() != dst.width())
            throw std::invalid_argument("Cannot merge sketches with different widths");

        bool touched_max = false;
        for (size_type j = 0uz; j < dst.width(); ++j) {
            if (src.S[j] < dst.S[j]) {
                if (dst.S[j] == *dst.max_s)
                    touched_max = true;
                dst.S[j] = src.S[j];
                dst.F[j] = src.F[j];
            }
        }

        if (touched_max)
            *dst.max_s = this->_row_max(dst.S);
    }

    void merge_min(const size_type dst_idx, const size_type src_idx) {
        this->merge_min(dst_idx, this->view_at(src_idx));
    }

    void reserve(const size_type n_vertices) {
        this->_id_map.reserve(n_vertices);
        this->_inv_ids.reserve(n_vertices);
        this->_max_s.reserve(n_vertices);
        this->_s.reserve(n_vertices * this->_m_stride);
        this->_f.reserve(n_vertices * this->_m_stride);
    }

    void clear() {
        this->_id_map.clear();
        this->_inv_ids.clear();
        this->_s.clear();
        this->_f.clear();
        this->_max_s.clear();
    }

private:
    void _verify_idx(const size_type idx) const {
        if (idx >= this->n_vertices())
            throw std::invalid_argument("Sketch row index out of range");
    }

    void _grow_to(const size_type n_vertices) {
        this->_max_s.resize(n_vertices);
        this->_s.resize(n_vertices * this->_m_stride);
        this->_f.resize(n_vertices * this->_m_stride);
    }

    void _init_row(const size_type idx) {
        const auto off = idx * this->_m_stride;
        for (size_type j = 0uz; j < this->_m; ++j) {
            this->_s[off + j] = sketch_infinity_v<weight_type>;
            this->_f[off + j] = edge_sample_type{};
        }
        this->_max_s[idx] = sketch_infinity_v<weight_type>;
    }

    [[nodiscard]] static weight_type _row_max(const std::span<const weight_type> S) {
        weight_type m = S.front();
        for (size_type j = 1uz; j < S.size(); ++j)
            m = std::max(m, S[j]);
        return m;
    }

    size_type _m;
    size_type _m_stride;

    std::unordered_map<id_type, size_type> _id_map;
    std::vector<id_type> _inv_ids;

    std::vector<weight_type> _s;
    std::vector<edge_sample_type> _f;
    std::vector<weight_type> _max_s;
};

} // namespace sgl
