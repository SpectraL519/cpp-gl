// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <algorithm>
#include <ranges>

namespace gl::util {

template <std::ranges::range R>
constexpr auto range_size(R&& r) {
    if constexpr (std::ranges::sized_range<R>)
        return std::ranges::size(r);
    else
        // Will consume input ranges!
        return std::ranges::distance(std::begin(r), std::end(r));
}

template <std::ranges::forward_range R>
[[nodiscard]] constexpr bool is_constant(R&& range) noexcept {
    if (std::ranges::empty(range))
        return true;

    return std::ranges::all_of(range, [target = *std::ranges::begin(range)](const auto& val) {
        return val == target;
    });
}

template <std::ranges::forward_range R>
[[nodiscard]] constexpr bool all_equal(R&& range, const std::ranges::range_value_t<R>& k) noexcept {
    if (std::ranges::empty(range))
        return true;

    return std::ranges::all_of(range, [&k](const auto& val) { return val == k; });
}

inline constexpr auto deref_view =
    std::views::transform([](auto&& p) -> decltype(auto) { return *p; });

// TODO: add tests

/// @brief A generic view that concatenates two ranges/views.
/// @todo Replace with `std::views::concat` (Requires C++26)
template <std::ranges::view V1, std::ranges::view V2>
class concat_view : public std::ranges::view_interface<concat_view<V1, V2>> {
public:
    concat_view() = default;

    constexpr concat_view(V1 v1, V2 v2) : _v1(std::move(v1)), _v2(std::move(v2)) {}

    constexpr auto begin() {
        return iterator<false>(
            std::ranges::begin(this->_v1),
            std::ranges::end(this->_v1),
            std::ranges::begin(this->_v2)
        );
    }

    constexpr auto begin() const
    requires std::ranges::range<const V1> && std::ranges::range<const V2>
    {
        return iterator<true>(
            std::ranges::begin(this->_v1),
            std::ranges::end(this->_v1),
            std::ranges::begin(this->_v2)
        );
    }

    constexpr auto end() {
        return sentinel<false>(std::ranges::end(this->_v2));
    }

    constexpr auto end() const
    requires std::ranges::range<const V1> && std::ranges::range<const V2>
    {
        return sentinel<true>(std::ranges::end(this->_v2));
    }

private:
    V1 _v1;
    V2 _v2;

    template <bool Const>
    class sentinel; // forward declare

    template <bool Const>
    class iterator {
    private:
        using BaseV1 = std::conditional_t<Const, const V1, V1>;
        using BaseV2 = std::conditional_t<Const, const V2, V2>;

    public:
        using difference_type = std::common_type_t<
            std::ranges::range_difference_t<BaseV1>,
            std::ranges::range_difference_t<BaseV2>>;
        using value_type = std::
            common_type_t<std::ranges::range_value_t<BaseV1>, std::ranges::range_value_t<BaseV2>>;
        using reference = std::common_reference_t<
            std::ranges::range_reference_t<BaseV1>,
            std::ranges::range_reference_t<BaseV2>>;
        using iterator_category = std::forward_iterator_tag;
        using iterator_concept = std::forward_iterator_tag;

        iterator() = default;

        constexpr iterator(
            std::ranges::iterator_t<BaseV1> it1,
            std::ranges::sentinel_t<BaseV1> end1,
            std::ranges::iterator_t<BaseV2> it2
        )
        : _it1(std::move(it1)), _end1(std::move(end1)), _it2(std::move(it2)) {}

        constexpr reference operator*() const {
            return (this->_it1 != this->_end1) ? *this->_it1 : *this->_it2;
        }

        constexpr iterator& operator++() {
            (this->_it1 != this->_end1) ? ++this->_it1 : ++this->_it2;
            return *this;
        }

        constexpr iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        constexpr bool operator==(const iterator& other) const {
            return this->_it1 == other._it1 && this->_it2 == other._it2;
        }

        constexpr bool operator==(const sentinel<Const>& s) const {
            return this->_it1 == this->_end1 && this->_it2 == s.end2();
        }

    private:
        std::ranges::iterator_t<BaseV1> _it1{};
        std::ranges::sentinel_t<BaseV1> _end1{};
        std::ranges::iterator_t<BaseV2> _it2{};

        friend class concat_view;
    };

    template <bool Const>
    class sentinel {
    private:
        using BaseV2 = std::conditional_t<Const, const V2, V2>;

    public:
        sentinel() = default;

        constexpr explicit sentinel(std::ranges::sentinel_t<BaseV2> end2)
        : _end2(std::move(end2)) {}

        constexpr auto end2() const {
            return this->_end2;
        }

    private:
        std::ranges::sentinel_t<BaseV2> _end2{};

        friend class concat_view;
        friend class iterator<Const>;
    };
};

namespace detail {

struct concat_fn {
    template <std::ranges::viewable_range R1, std::ranges::viewable_range R2>
    constexpr auto operator()(R1&& r1, R2&& r2) const {
        return concat_view<std::views::all_t<R1>, std::views::all_t<R2>>(
            std::views::all(std::forward<R1>(r1)), std::views::all(std::forward<R2>(r2))
        );
    }
};

} // namespace detail

/// @brief Concatenates two ranges sequentially into a single view.
/// @todo replace with `std::views::concat`
inline constexpr detail::concat_fn concat{};

} // namespace gl::util
