// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/util/ranges.hpp
/// @brief Defines utility functions and views for working with C++20 ranges.

#pragma once

#include <algorithm>
#include <ranges>

namespace gl::util {

/// @ingroup GL-Util
/// @brief A transformation callback that returns a descriptor's ID.
inline constexpr auto to_id = [](const auto& descriptor) { return descriptor.id(); };

/// @ingroup GL-Util
/// @brief Safely determines the size of a range.
///
/// This function returns the size of a range if it is a sized range, otherwise it computes the
/// distance between the beginning and end of the range. Note that computing the distance for
/// non-sized ranges may be expensive.
///
/// > [!WARNING] This function will consume input ranges that are not sized, as it needs to iterate through them to count the elements.
///
/// @tparam R The type of the range.
/// @param r The range to measure.
/// @return The size of the range if it is a sized range, otherwise the distance between the beginning and end of the range.
///
/// > [!INFO] Time Complexity
/// >
/// > $O(1)$ if the range is a sized range, otherwise $O(N)$ where $N$ is the number of elements in the range.
template <std::ranges::range R>
constexpr auto range_size(R&& r) {
    if constexpr (std::ranges::sized_range<R>)
        return std::ranges::size(r);
    else
        // Will consume input ranges!
        return std::ranges::distance(std::begin(r), std::end(r));
}

/// @ingroup GL-Util
/// @brief Checks if all elements in a range are equal.
/// @tparam R The type of the range.
/// @param range The range to check.
/// @return `true` if all elements in the range are equal or the range is empty, otherwise `false`.
///
/// > [!INFO] Time Complexity
/// >
/// > $O(N)$ where $N$ is the number of elements in the range.
template <std::ranges::forward_range R>
[[nodiscard]] constexpr bool is_constant(R&& range) noexcept {
    if (std::ranges::empty(range))
        return true;

    return std::ranges::all_of(range, [target = *std::ranges::begin(range)](const auto& val) {
        return val == target;
    });
}

/// @ingroup GL-Util
/// @brief Checks if all elements in a range are equal to a given value.
/// @tparam R The type of the range.
/// @param range The range to check.
/// @param value The value to compare against.
/// @return `true` if all elements in the range are equal to the given value or the range is empty, otherwise `false`.
///
/// > [!INFO] Time Complexity
/// >
/// > \f$O(N)\f$ where $N$ is the number of elements in the range.
template <std::ranges::forward_range R>
[[nodiscard]] constexpr bool all_equal(
    R&& range, const std::ranges::range_value_t<R>& value
) noexcept {
    if (std::ranges::empty(range))
        return true;

    return std::ranges::all_of(range, [&value](const auto& val) { return val == value; });
}

/// @ingroup GL-Util
/// @brief A view concatenating two ranges sequentially (C++20 polyfill for C++26 `std::views::concat`).
///
/// > [!WARNING] GCC 13/14 Bug
/// >
/// > Using branching views (like this or `std::ranges::filter_view`) inside complex algorithms
/// > (e.g., `std::ranges::is_permutation`) may trigger false-positive `-Wmaybe-uninitialized`
/// > warnings. To work around this, suppress the warning at the call site or materialize the
/// > view into a contiguous container like `std::vector`.
///
/// @tparam V1 First view type.
/// @tparam V2 Second view type.
/// @todo Replace with `std::views::concat` (C++26).
/// ### See Also
/// - @ref gl::util::concat_fn "concat_fn": A helper compile-time constant function object for creating `concat_view` instances.
/// - @ref gl::util::concat "concat": A compile-time constant instantiation of this function object for convenient use.
template <std::ranges::view V1, std::ranges::view V2>
class concat_view : public std::ranges::view_interface<concat_view<V1, V2>> {
public:
    /// @brief Default constructor creates an empty concatenated view.
    concat_view() = default;

    /// @brief Constructs a `concat_view` from two viewable ranges.
    /// @param v1 The first range to concatenate.
    /// @param v2 The second range to concatenate.
    constexpr concat_view(V1 v1, V2 v2) : _v1(std::move(v1)), _v2(std::move(v2)) {}

    /// @brief Returns an iterator to the beginning of the concatenated view.
    /// @return An iterator that traverses the first range followed by the second range.
    constexpr auto begin() {
        return iterator<false>(
            std::ranges::begin(this->_v1),
            std::ranges::end(this->_v1),
            std::ranges::begin(this->_v2)
        );
    }

    /// @brief Returns a const iterator to the beginning of the concatenated view.
    /// @return A const iterator that traverses the first range followed by the second range.
    constexpr auto begin() const
    requires(std::ranges::range<const V1> and std::ranges::range<const V2>)
    {
        return iterator<true>(
            std::ranges::begin(this->_v1),
            std::ranges::end(this->_v1),
            std::ranges::begin(this->_v2)
        );
    }

    /// @brief Returns a sentinel representing the end of the concatenated view.
    /// @return A sentinel that compares equal to an iterator when it reaches the end of the second range.
    constexpr auto end() {
        return sentinel<false>(std::ranges::end(this->_v2));
    }

    /// @brief Returns a const sentinel representing the end of the concatenated view.
    /// @return A const sentinel that compares equal to a const iterator when it reaches the end
    constexpr auto end() const
    requires(std::ranges::range<const V1> and std::ranges::range<const V2>)
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
            if (this->_it1 != this->_end1)
                ++this->_it1;
            else
                ++this->_it2;
            return *this;
        }

        constexpr iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        constexpr bool operator==(const iterator& other) const {
            return this->_it1 == other._it1 and this->_it2 == other._it2;
        }

        constexpr bool operator==(const sentinel<Const>& s) const {
            return this->_it1 == this->_end1 and this->_it2 == s.end2();
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

/// @ingroup GL-Util
/// @brief A function object for concatenating two viewable ranges into a `concat_view`.
/// ### See Also
/// - @ref gl::util::concat_view "concat_view": The view type that represents the concatenation of two ranges.
/// - @ref gl::util::concat "concat": A compile-time constant instantiation of this function object for convenient use.
struct concat_fn {
    /// @brief Concatenates two viewable ranges into a `concat_view`.
    /// @tparam R1 The type of the first range.
    /// @tparam R2 The type of the second range.
    /// @param r1 The first range to concatenate.
    /// @param r2 The second range to concatenate.
    /// @return A `concat_view` that represents the concatenation of the two ranges.
    template <std::ranges::viewable_range R1, std::ranges::viewable_range R2>
    constexpr auto operator()(R1&& r1, R2&& r2) const {
        return concat_view<std::views::all_t<R1>, std::views::all_t<R2>>(
            std::views::all(std::forward<R1>(r1)), std::views::all(std::forward<R2>(r2))
        );
    }
};

/// @ingroup GL-Util
/// @brief Concatenates two viewable ranges into a `concat_view`.
///
/// ### Example usage
/// ```cpp
/// std::vector<int> v1 = {1, 2, 3};
/// std::vector<int> v2 = {4, 5, 6};
/// auto concatenated = gl::util::concat(v1, v2);
/// for (int x : concatenated)
///     std::cout << x << " "; // Output: 1 2 3 4 5 6
/// ```
///
/// @param r1 First range to concatenate.
/// @param r2 Second range to concatenate.
/// @todo Replace with `std::views::concat` (C++26).
/// ### See Also
/// - @ref gl::util::concat_view "concat_view": The view type that represents the concatenation of two ranges.
/// - @ref gl::util::concat_fn "concat_fn": A helper compile-time constant function object for creating `concat_view` instances.
inline constexpr concat_fn concat{};

} // namespace gl::util
