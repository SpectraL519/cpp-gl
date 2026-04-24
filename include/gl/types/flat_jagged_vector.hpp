// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/types/flat_jagged_vector.hpp
/// @brief Contains the implementation of a generic @ref gl::flat_jagged_vector "flat_jagged_vector" data structure.

#pragma once

#include "gl/types/core.hpp"

#include <concepts>
#include <cstdint>
#include <format>
#include <initializer_list>
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>

namespace gl {

/// @ingroup GL GL-Types
/// @brief A flattened 2D vector (jagged array) providing efficient storage for variable-length segments.
///
/// This container stores all elements in a single contiguous memory block (*data*) while maintaining
/// an offsets array (*offsets*) to track segment boundaries. This design provides excellent cache locality
/// and efficient iteration over individual segments.
///
/// ### Example Usage
/// ```cpp
/// #include <gl/types/flat_jagged_vector.hpp>
///
/// #include <iostream>
///
/// int main() {
///     gl::flat_jagged_vector<int> vec = { // (1)!
///         {1, 2, 3},
///         {4, 5},
///         {6, 7, 8, 9}
///     };
///
///     vec.push_back({10, 11}); // (2)!
///     vec.push_back(1uz, 99); // (3)!
///
///     for (const auto [i, segment] : vec) { // (4)!
///         std::cout << "Segment " << i << ": ";
///         for (int value : segment)
///             std::cout << value << " ";
///         std::cout << '\n';
///     }
///
///     return 0;
/// }
/// ```
///
/// 1\. Initialize the `flat_jagged_vector` with an initializer list of segments, where each segment can have a different length.
///
/// 2\. Append a new segment to the end of the container using `push_back()`.
///
/// 3\. Append an element to the second segment (index 1).
///
/// 4\. Iterate over the segments and their elements using the `operator[]` to access each segment as a range.
///
/// **Output:**
/// ```text
/// Segment 0: 1 2 3
/// Segment 1: 4 5 99
/// Segment 2: 6 7 8 9
/// Segment 3: 10 11
/// ```
///
/// > [!NOTE] Container behaviour
/// >
/// > Behavior is similar to `std::vector<std::vector<T>>` but with flattened memory layout.
///
/// > [!IMPORTANT] Iterator Invalidation Policy
/// >
/// > Iterator invalidation follows `std::vector` semantics: modifying the structure or elements
/// > invalidates all iterators, pointers, and references to the container's elements.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | T         | The type of the elements | Must be **semiregular** (default constructible and copyable) |
///
/// ### TODO
/// - Implement assign, and swap methods.
/// - Implement iterator-based insert, emplace and erase methods.
/// - Add `operator<<` overload for `std::ostream` and specialize `std::formatter`.
/// - Use `std::ptrdiff_t` instead of `std::size_t` for offset values.
template <std::semiregular T>
class flat_jagged_vector {
public:
    /// @brief Type of elements stored in segments.
    using value_type = T;
    /// @brief Unsigned integral type used for sizes and indices.
    using size_type = std::size_t;
    /// @brief The underlying contiguous storage container.
    using container_type = std::vector<value_type>;
    /// @brief Reference to an element.
    using reference = typename container_type::reference;
    /// @brief Const reference to an element.
    using const_reference = typename container_type::const_reference;
    /// @brief Subrange type representing a non-owning segment of elements.
    using segment_type = std::ranges::subrange<typename container_type::iterator>;
    /// @brief Const subrange type representing a non-owning const segment of elements.
    using const_segment_type = std::ranges::subrange<typename container_type::const_iterator>;

    // --- iterators ---

    /// @brief Random access iterator over segments of the `flat_jagged_vector`.
    ///
    /// This iterator dereferences to a `segment_type` (subrange of elements in a single segment),
    /// allowing efficient iteration and random access to individual segments. The iterator maintains
    /// pointers to the element data and the offsets array for dereferencing.
    ///
    /// ### Template Parameters
    /// | Parameter | Type | Description |
    /// | :-------- | :--- | :--- |
    /// | Const     | `bool` | If `true`, produces const iterators; if `false`, produces mutable iterators. |
    ///
    /// > [!NOTE] Complexity
    /// >
    /// > Provides random access semantics: \f$O(1)\f$ for all operations.
    ///
    /// > [!Caution] Invalidation
    /// >
    /// > Invalidated when the referenced @ref gl::flat_jagged_vector "flat_jagged_vector" is modified (structure changes or element insertions/deletions).
    ///
    /// @see gl::flat_jagged_vector
    template <bool Const>
    class segment_iterator {
        using data_iter_type = std::conditional_t<
            Const,
            typename container_type::const_iterator,
            typename container_type::iterator>;
        using offset_ptr_type = const size_type*;

    public:
        /// @brief Satisfies random access iterator concept.
        using iterator_concept = std::random_access_iterator_tag;
        /// @brief Legacy iterator category (random access).
        using iterator_category = std::random_access_iterator_tag;
        /// @brief Type of segment this iterator dereferences to (subrange or const subrange).
        using value_type = std::conditional_t<Const, const_segment_type, segment_type>;
        /// @brief Signed integral difference type.
        using difference_type = std::ptrdiff_t;
        /// @brief Pointer type (void because segment iterators dereference to subranges).
        using pointer = void;
        /// @brief Reference type (subrange of elements).
        using reference = value_type;

        /// @brief Default constructor creates a null iterator.
        segment_iterator() = default;

        /// @brief Constructs an iterator pointing to a specific segment.
        /// @param data_iter  Iterator to the underlying element data (may be null for null iterator).
        /// @param offset_ptr Pointer to the offsets array at the position of this segment.
        segment_iterator(data_iter_type data_iter, offset_ptr_type offset_ptr) noexcept
        : _data_iter(data_iter), _offset_ptr(offset_ptr) {}

        /// @brief Implicit conversion from mutable to const iterator.
        /// @return A const iterator pointing to the same segment.
        operator segment_iterator<true>() const noexcept
        requires(not Const)
        {
            return segment_iterator<true>(this->_data_iter, this->_offset_ptr);
        }

        /// @brief Dereferences the iterator to the current segment.
        /// @return A subrange representing the segment at the current position.
        [[nodiscard]] reference operator*() const noexcept {
            return reference(
                this->_data_iter + to_diff(*this->_offset_ptr),
                this->_data_iter + to_diff(*(this->_offset_ptr + 1uz))
            );
        }

        /// @brief Random access to a segment at offset from current position.
        /// @param n Offset (can be negative).
        /// @return Segment at offset n from the current position.
        /// @pre `0 <= current_position + n < container.size()`
        /// > [!WARNING] The operation results in Undefined Behavior if the precondition is not satisfied.
        [[nodiscard]] reference operator[](difference_type n) const noexcept {
            return *(*this + n);
        }

        /// @brief Pre-increment operator.
        /// @return Reference to this iterator after advancing to the next segment.
        segment_iterator& operator++() noexcept {
            ++this->_offset_ptr;
            return *this;
        }

        /// @brief Post-increment operator.
        /// @return A copy of this iterator before the increment.
        segment_iterator operator++(int) noexcept {
            auto tmp = *this;
            ++this->_offset_ptr;
            return tmp;
        }

        /// @brief Pre-decrement operator.
        /// @return Reference to this iterator after moving to the previous segment.
        segment_iterator& operator--() noexcept {
            --this->_offset_ptr;
            return *this;
        }

        /// @brief Post-decrement operator.
        /// @return A copy of this iterator before the decrement.
        segment_iterator operator--(int) noexcept {
            auto tmp = *this;
            --this->_offset_ptr;
            return tmp;
        }

        /// @brief Advances the iterator by n positions.
        /// @param n Number of segments to advance (can be negative).
        /// @return Reference to this iterator.
        segment_iterator& operator+=(difference_type n) noexcept {
            this->_offset_ptr += n;
            return *this;
        }

        /// @brief Moves the iterator backward by n positions.
        /// @param n Number of segments to move backward (can be negative).
        /// @return Reference to this iterator.
        segment_iterator& operator-=(difference_type n) noexcept {
            this->_offset_ptr -= n;
            return *this;
        }

        /// @brief Creates a new iterator advanced by n positions from the given iterator.
        /// @param it Iterator to advance from.
        /// @param n Number of segments to advance.
        /// @return New iterator at the advanced position.
        [[nodiscard]] friend segment_iterator operator+(
            segment_iterator it, difference_type n
        ) noexcept {
            return it += n;
        }

        /// @brief Creates a new iterator advanced by n positions (commutative form).
        /// @param n Number of segments to advance.
        /// @param it Iterator to advance from.
        /// @return New iterator at the advanced position.
        [[nodiscard]] friend segment_iterator operator+(
            difference_type n, segment_iterator it
        ) noexcept {
            return it += n;
        }

        /// @brief Creates a new iterator moved backward by n positions.
        /// @param it Iterator to move backward from.
        /// @param n Number of segments to move backward.
        /// @return New iterator at the moved position.
        [[nodiscard]] friend segment_iterator operator-(
            segment_iterator it, difference_type n
        ) noexcept {
            return it -= n;
        }

        /// @brief Computes the distance between two iterators.
        /// @param lhs The later iterator.
        /// @param rhs The earlier iterator.
        /// @return Number of segments between the iterators; negative if `lhs < rhs`.
        [[nodiscard]] friend difference_type operator-(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr - rhs._offset_ptr;
        }

        /// @brief Tests equality of two iterators.
        /// @param lhs Left iterator.
        /// @param rhs Right iterator.
        /// @return `true` if both iterators point to the same segment.
        [[nodiscard]] friend bool operator==(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr == rhs._offset_ptr;
        }

        /// @brief Three-way comparison of two iterators.
        /// @param lhs Left iterator.
        /// @param rhs Right iterator.
        /// @return Comparison result indicating iterator ordering.
        [[nodiscard]] friend auto operator<=>(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr <=> rhs._offset_ptr;
        }

    private:
        data_iter_type _data_iter;
        offset_ptr_type _offset_ptr{nullptr};
    };

    /// @brief Mutable random access iterator over segments.
    using iterator = segment_iterator<false>;
    /// @brief Const random access iterator over segments.
    using const_iterator = segment_iterator<true>;
    /// @brief Reverse mutable iterator.
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// @brief Reverse const iterator.
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    /// @brief Default constructor creates an empty `flat_jagged_vector`.
    /// @post `empty() == true and size() == 0 and data_size() == 0`
    flat_jagged_vector() = default;

    /// @brief Copy constructor creates a deep copy of another `flat_jagged_vector`.
    /// @param other The `flat_jagged_vector` to copy.
    /// @post `*this == other`
    flat_jagged_vector(const flat_jagged_vector&) = default;

    /// @brief Copy assignment creates a deep copy of another `flat_jagged_vector`.
    /// @param other The source `flat_jagged_vector`.
    /// @return Reference to `*this`.
    /// @post `*this == other`
    flat_jagged_vector& operator=(const flat_jagged_vector&) = default;

    /// @brief Move constructor transfers ownership of data from another `flat_jagged_vector`.
    /// @param other The source `flat_jagged_vector` (left in a valid but unspecified state).
    ///
    /// ### Postconditions:
    ///
    /// 1. `other.empty() == true`
    /// 2. All data from `other` is transferred to `*this`
    ///
    /// > [!WARNING] Invalidates all iterators, pointers, and references to other instance's elements.
    flat_jagged_vector(flat_jagged_vector&& other) noexcept
    : _data(std::move(other._data)), _offsets(std::move(other._offsets)) {
        other._offsets = {0uz};
    }

    /// @brief Move assignment transfers ownership of data from another `flat_jagged_vector`.
    /// @param other The source `flat_jagged_vector`.
    /// @return Reference to `*this`.
    /// ### Postconditions:
    /// 1. `other.empty() == true`
    /// 2. All data from `other` is transferred to `*this`
    ///
    /// > [!INFO] Safety
    /// >
    /// > This function safely handles self-assignment and if `this != &other`
    /// > the operator will leave the `other` instance in a valid, empty state.
    ///
    /// > [!WARNING] Invalidates all iterators, pointers, and references to this container's elements.
    flat_jagged_vector& operator=(flat_jagged_vector&& other) noexcept {
        if (this != &other) {
            this->_data = std::move(other._data);
            this->_offsets = std::move(other._offsets);
            other._offsets = {0uz};
        }
        return *this;
    }

    /// @brief Constructs a `flat_jagged_vector` with a specified number of segments and initial segment size.
    /// @param n_segments The number of segments to create.
    /// @param segment_size The initial size of each segment (default is 0).
    /// @post `size() == n_segments` and each segment is initialized with `segment_size` default-constructed elements.
    /// @throws std::bad_alloc May throw if memory allocation fails.
    flat_jagged_vector(size_type n_segments, size_type segment_size = 0uz)
    : _data(n_segments * segment_size), _offsets(n_segments + 1uz) {
        for (size_type i = 0uz; i <= n_segments; i++)
            this->_offsets[i] = i * segment_size;
    }

    /// @brief Constructs a `flat_jagged_vector` from an initializer list of segments.
    /// @param ilist Initializer list of initializer lists, each representing a segment.
    /// @post `size() == ilist.size()` and `data_size()` equals the sum of all segment sizes.
    /// @throws std::bad_alloc May throw if memory allocation fails.
    flat_jagged_vector(std::initializer_list<std::initializer_list<value_type>> ilist) {
        this->reserve_segments(ilist.size());

        size_type total_size = 0uz;
        for (const auto& sub : ilist)
            total_size += sub.size();
        this->reserve_data(total_size);

        for (const auto& sub : ilist)
            this->push_back(sub);
    }

    /// @brief Constructs a `flat_jagged_vector` from a range of ranges.
    ///
    /// This constructor accepts any input range of input ranges convertible to `value_type`,
    /// enabling flexible initialization from various container types.
    ///
    /// @tparam R A range type whose elements are input ranges of `value_type`.
    /// @param  r The range of ranges to initialize from.
    /// @post `size()` equals the number of outer range elements; `data_size()` is the sum of all element counts.
    /// @throws std::bad_alloc May throw if memory allocation fails.
    template <std::ranges::input_range R>
    requires(std::ranges::input_range<std::ranges::range_reference_t<R>> and std::convertible_to<std::ranges::range_reference_t<std::ranges::range_reference_t<R>>, value_type>)
    explicit flat_jagged_vector(R&& r) {
        if constexpr (std::ranges::sized_range<R>)
            this->reserve_segments(std::ranges::size(r));

        for (auto&& subrange : r)
            this->push_back(std::forward<decltype(subrange)>(subrange));
    }

    /// @brief Destructor cleans up all managed memory.
    ~flat_jagged_vector() = default;

    // --- comparsion ---

    /// @brief Tests equality of two `flat_jagged_vector` instances.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return `true` if both vectors have the same structure and elements.
    friend bool operator==(const flat_jagged_vector&, const flat_jagged_vector&) = default;

    // --- size and capacity ---

    /// @brief Returns the number of segments in this container.
    /// @return The number of segments.
    [[nodiscard]] size_type size() const noexcept {
        return this->_offsets.size() - 1uz;
    }

    /// @brief Checks if the container is empty (contains no segments).
    /// @return `true` if there are no elements in the container, `false` otherwise.
    [[nodiscard]] bool empty() const noexcept {
        return this->size() == 0uz;
    }

    /// @brief Returns the current capacity for segments (number of segment slots allocated).
    /// @return The number of segments that can be stored without reallocation of the *offsets* storage.
    [[nodiscard]] size_type segments_capacity() const noexcept {
        return this->_offsets.capacity() - 1uz;
    }

    /// @brief Returns the current capacity for data elements.
    /// @return The number of elements that can be stored in *data* storage without reallocation.
    [[nodiscard]] size_type data_capacity() const noexcept {
        return this->_data.capacity();
    }

    /// @brief Reserves space for at least n additional segments in the *offsets* storage without changing the size.
    /// @param n The number of segments to reserve space for.
    /// @post `segments_capacity() >= n + size()`
    /// > [!WARNING] Invalidates all iterators and pointers to elements if reallocation occurs.
    void reserve_segments(size_type n) {
        this->_offsets.reserve(n + 1uz);
    }

    /// @brief Reserves space for at least n additional data elements without changing the size.
    /// @param n The number of elements to reserve space for.
    /// @post `data_capacity() >= n + data_size()`
    /// > [!WARNING] Invalidates all iterators and pointers to elements if reallocation occurs.
    void reserve_data(size_type n) {
        this->_data.reserve(n);
    }

    /// @brief Reduces capacity of both internal arrays to match current size.
    /// ### Postconditions
    /// 1. `segments_capacity() == size()`
    /// 2. `data_capacity() == data_size()`
    /// > [!WARNING] Invalidates all iterators, pointers, and references to elements.
    void shrink_to_fit() {
        this->_data.shrink_to_fit();
        this->_offsets.shrink_to_fit();
    }

    /// @brief Resizes the container to contain `n` segments.
    ///
    /// - If the current size is greater than `n`, the container is reduced to its first `n` segments.
    /// - If the current size is less than `n`, additional empty segments are appended.
    /// - If the current size is equal to `n`, the container is unchanged.
    ///
    /// @param n The new number of segments.
    /// @post `size() == n`.
    ///
    /// > [!WARNING] Iterator Invalidation
    /// >
    /// > This operation invalidates all iterators, pointers, and references to elements if reallocation
    /// > occurs, or if the container shrinks (invalidating removed segments).
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > $O(E)$ when shrinking (where $E$ is the total number of elements in the removed segments),
    /// > or amortized $O(S)$ when growing (where $S$ is the number of new empty segments).
    void resize(size_type n) {
        if (n < this->size()) {
            this->_offsets.resize(n + 1uz);
            this->_data.resize(this->_offsets.back());
        }
        else if (n > this->size()) {
            this->_offsets.resize(n + 1uz, this->_offsets.back());
        }
    }

    /// @brief Resizes the container to contain `n` segments, initializing any new segments with a range.
    ///
    /// - If the current size is greater than `n`, the container is reduced to its first `n` segments.
    /// - If the current size is less than `n`, new segments are appended, each containing the elements in `r`.
    /// - If the current size is equal to `n`, the container is unchanged.
    ///
    /// @tparam R An input range of elements convertible to `value_type`.
    /// @param n The new number of segments.
    /// @param r The range to initialize any newly appended segments with.
    /// @post `size() == n`
    /// @throws std::bad_alloc If memory allocation fails.
    ///
    /// > [!WARNING] Iterator Invalidation
    /// >
    /// > This operation invalidates all iterators, pointers, and references to elements if reallocation
    /// > occurs, or if the container shrinks (invalidating removed segments).
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > $O(E)$ when shrinking (where $E$ is the total number of elements in the removed segments),
    /// > or amortized \f$O(S \cdot E_r)\f$ when growing (where $S$ is the number of new empty segments
    /// > and \f$E_r\f$ is the number of elements in the input range `r`).
    template <std::ranges::input_range R>
    void resize(size_type n, R&& r)
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    {
        if (n < this->size()) {
            this->_offsets.resize(n + 1uz);
            this->_data.resize(this->_offsets.back());
        }
        else if (n > this->size()) {
            const auto diff = n - this->size();
            this->reserve_segments(n);

            this->push_back(std::forward<R>(r));
            if (diff > 1uz) {
                // prevent reallocation during loop
                const auto seg_size = this->back().size();
                this->reserve_data(this->data_size() + seg_size * (diff - 1uz));
                for (size_type i = 1uz; i < diff; ++i)
                    this->push_back(this->back());
            }
        }
    }

    /// @brief Resizes the container to contain `n` segments, initializing any new segments with an initializer list.
    ///
    /// - If the current size is greater than `n`, the container is reduced to its first `n` segments.
    /// - If the current size is less than `n`, new segments are appended, each containing the elements in `ilist`.
    /// - If the current size is equal to `n`, the container is unchanged.
    ///
    /// @param n The new number of segments.
    /// @param ilist The initializer list to initialize any newly appended segments with.
    ///
    /// > [!INFO] Implementation
    /// >
    /// > This function is equivalent to calling `resize(n, std::span<const value_type>{ilist})`.
    void resize(size_type n, std::initializer_list<value_type> ilist) {
        this->resize(n, std::span<const value_type>{ilist});
    }

    /// @brief Removes all segments and elements, leaving the container empty.
    /// @post `empty() == true` but the capacity remains unchanged.
    /// > [!WARNING] Invalidates all iterators, pointers, and references to elements.
    void clear() {
        this->_data.clear();
        this->_offsets.clear();
        this->_offsets.push_back(0uz);
    }

    // --- accessors ---

    /// @brief Returns the segment at the given index without bounds checking.
    /// @param  i The index of the segment to access.
    /// @return   A subrange representing the segment at index i.
    /// @pre `seg < size()`
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed for performance. Use `at(seg)` for bounds-checked access.
    /// > Calling on an out-of-bounds index results in Undefined Behavior.
    [[nodiscard]] segment_type operator[](size_type seg) {
        return segment_type(
            this->_data.begin() + to_diff(this->_offsets[seg]),
            this->_data.begin() + to_diff(this->_offsets[seg + 1uz])
        );
    }

    /// @brief Returns a const segment at the given index without bounds checking.
    /// @param  seg The index of the segment to access.
    /// @return A const subrange representing the segment at index i.
    /// @pre `seg < size()`
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed for performance. Use `at(seg)` for bounds-checked access.
    /// > Calling on an out-of-bounds index results in Undefined Behavior.
    [[nodiscard]] const_segment_type operator[](size_type seg) const {
        return const_segment_type(
            this->_data.begin() + to_diff(this->_offsets[seg]),
            this->_data.begin() + to_diff(this->_offsets[seg + 1uz])
        );
    }

    /// @brief Returns a reference to an element within a segment without bounds checking.
    /// @param seg The segment number.
    /// @param pos The position within the segment.
    /// @return Reference to the element at the given segment and position.
    /// @pre `seg < size()` and `pos < segment_size(seg)`
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed for performance. Use `at(seg, pos)` for bounds-checked access.
    /// > Out-of-bounds access results in Undefined Behavior.
    [[nodiscard]] reference operator[](size_type seg, size_type pos) {
        return this->_data[this->_offsets[seg] + pos];
    }

    /// @brief Returns a const reference to an element within a segment without bounds checking.
    /// @param seg The segment number.
    /// @param pos The position within the segment.
    /// @return Const reference to the element at the given segment and position.
    /// @pre `seg < size()` and `pos < segment_size(seg)`
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed for performance. Use `at(seg, pos)` for bounds-checked access.
    /// > Out-of-bounds access results in Undefined Behavior.
    [[nodiscard]] const_reference operator[](size_type seg, size_type pos) const {
        return this->_data[this->_offsets[seg] + pos];
    }

    /// @brief Returns the segment at the given index with bounds checking.
    /// @param seg The index of the segment.
    /// @return A subrange representing the segment at index `seg`.
    /// @throws std::out_of_range If `seg >= size()`.
    [[nodiscard]] segment_type at(size_type seg) {
        this->_check_range(seg);
        return (*this)[seg];
    }

    /// @brief Returns a const segment at the given index with bounds checking.
    /// @param seg The index of the segment.
    /// @return A const subrange representing the segment at index `seg`.
    /// @throws std::out_of_range If `seg >= size()`.
    [[nodiscard]] const_segment_type at(size_type seg) const {
        this->_check_range(seg);
        return (*this)[seg];
    }

    /// @brief Returns a reference to an element within a segment with bounds checking.
    /// @param seg The segment number.
    /// @param pos The position within the segment.
    /// @return Reference to the element at the given segment and position.
    /// @throws std::out_of_range If `seg >= size()` or `pos >= segment_size(seg)`.
    [[nodiscard]] reference at(size_type seg, size_type pos) {
        this->_check_range(seg);
        this->_check_segment_range(seg, pos);
        return (*this)[seg, pos];
    }

    /// @brief Returns a const reference to an element within a segment with bounds checking.
    /// @param seg The segment number.
    /// @param pos The position within the segment.
    /// @return Const reference to the element at the given segment and position.
    /// @throws std::out_of_range If `seg >= size()` or `pos >= segment_size(seg)`.
    [[nodiscard]] const_reference at(size_type seg, size_type pos) const {
        this->_check_range(seg);
        this->_check_segment_range(seg, pos);
        return (*this)[seg, pos];
    }

    /// @brief Returns the first segment without bounds checking.
    /// @return A subrange representing the first segment.
    /// @pre Container must not be empty
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if container is empty.
    [[nodiscard]] segment_type front() noexcept {
        return (*this)[0uz];
    }

    /// @brief Returns a const reference to the first segment without bounds checking.
    /// @return A const subrange representing the first segment.
    /// @pre Container must not be empty.
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if container is empty.
    [[nodiscard]] const_segment_type front() const noexcept {
        return (*this)[0uz];
    }

    /// @brief Returns the last segment without bounds checking.
    /// @return A subrange representing the last segment.
    /// @pre Container must not be empty.
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if container is empty.
    [[nodiscard]] segment_type back() noexcept {
        return (*this)[this->size() - 1uz];
    }

    /// @brief Returns a const reference to the last segment without bounds checking.
    /// @return A const subrange representing the last segment.
    /// @pre Container must not be empty.
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if container is empty.
    [[nodiscard]] const_segment_type back() const noexcept {
        return (*this)[this->size() - 1uz];
    }

    /// @brief Returns a reference to the first element in a segment without bounds checking.
    /// @param  seg The segment number.
    /// @return  Reference to the first element in the segment.
    /// @pre `seg < size()` and the segment must not be empty.
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if the precondition is not satisfied.
    [[nodiscard]] reference front(size_type seg) noexcept {
        return (*this)[seg, 0uz];
    }

    /// @brief Returns a const reference to the first element in a segment without bounds checking.
    /// @param  seg The segment number.
    /// @return Const reference to the first element in the segment.
    /// @pre `seg < size()` and the segment must not be empty.
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if the precondition is not satisfied.
    [[nodiscard]] const_reference front(size_type seg) const noexcept {
        return (*this)[seg, 0uz];
    }

    /// @brief Returns a reference to the last element in a segment without bounds checking.
    /// @param seg The segment number.
    /// @return Reference to the last element in the segment.
    /// @pre `seg < size()` and the segment must not be empty.
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if the precondition is not satisfied.
    [[nodiscard]] reference back(size_type seg) noexcept {
        return (*this)[seg, this->segment_size(seg) - 1uz];
    }

    /// @brief Returns a const reference to the last element in a segment without bounds checking.
    /// @param seg The segment number.
    /// @return Const reference to the last element in the segment.
    /// @pre `seg < size()` and the segment must not be empty.
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if the precondition is not satisfied.
    [[nodiscard]] const_reference back(size_type seg) const noexcept {
        return (*this)[seg, this->segment_size(seg) - 1uz];
    }

    /// @brief Returns a view of all segments for iteration.
    /// @return A range-adaptable, random-access view of all segments.
    /// > [!NOTE] This creates a lazy view; iterating yields segments as subranges.
    [[nodiscard]] auto segments() noexcept {
        return std::views::iota(size_type{0}, this->size())
             | std::views::transform([this](size_type i) -> segment_type { return (*this)[i]; });
    }

    /// @brief Returns a const view of all segments for iteration.
    /// @return A const range-adaptable, random-access view of all segments.
    /// > [!NOTE] This creates a lazy view; iterating yields const segments as const subranges.
    [[nodiscard]] auto segments() const noexcept {
        return std::views::iota(size_type{0}, this->size())
             | std::views::transform([this](size_type i) -> const_segment_type {
                   return (*this)[i];
               });
    }

    /// @brief Checks if a specific segment is empty without bounds checking.
    /// @param seg The segment number.
    /// @return `true` if the segment is empty, `false` otherwise.
    /// @pre `seg < size()`
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if segment index is out of bounds.
    [[nodiscard]] bool empty(size_type seg) const noexcept {
        return this->_offsets[seg] == this->_offsets[seg + 1uz];
    }

    /// @brief Returns the number of elements in a specific segment without bounds checking.
    /// @param seg The segment number.
    /// @return The count of elements in the segment.
    /// @pre `seg < size()`
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if segment index is out of bounds.
    [[nodiscard]] size_type segment_size(size_type seg) const noexcept {
        return this->_offsets[seg + 1uz] - this->_offsets[seg];
    }

    /// @brief Returns the total number of elements across all segments.
    /// @return The sum of sizes of all segments.
    [[nodiscard]] size_type data_size() const noexcept {
        return this->_data.size();
    }

    /// @brief Returns a subrange of all element data in flattened form.
    /// @return A subrange of all elements in the underlying *data* array.
    /// > [!NOTE] Allows direct access to the flattened data representation.
    [[nodiscard]] segment_type data_view() noexcept {
        return segment_type(this->_data);
    }

    /// @brief Returns a const subrange of all element data in flattened form.
    /// @return A const subrange of all elements in the underlying *data* array.
    /// > [!NOTE] Allows direct access to the flattened data representation.
    [[nodiscard]] const_segment_type data_view() const noexcept {
        return const_segment_type(this->_data);
    }

    /// @brief Returns a reference to the underlying flat data container.
    /// @return A mutable reference to the underlying *data* array.
    ///
    /// > [!CAUTION] Use with extreme caution
    /// >
    /// > Modifying this vector directly can corrupt the structure of the container. If possible,
    /// > use `data_view()` instead. This method is intended for advanced operations.
    [[nodiscard]] std::vector<value_type>& data_storage() noexcept {
        return this->_data;
    }

    /// @brief Returns a const reference to the underlying flat data container.
    /// @return A const reference to the underlying *data* array.
    [[nodiscard]] const std::vector<value_type>& data_storage() const noexcept {
        return this->_data;
    }

    /// @brief Returns a raw pointer to the underlying flat data array.
    /// @return A raw pointer to the first element in the *data* array.
    /// > [!IMPORTANT] Not available for boolean flat vectors.
    [[nodiscard]] value_type* data_ptr() noexcept
    requires(not std::same_as<value_type, bool>)
    {
        return this->_data.data();
    }

    /// @brief Returns a const raw pointer to the underlying flat data array.
    /// @return A const raw pointer to the first element in the *data* array.
    /// > [!IMPORTANT] Not available for boolean flat vectors.
    [[nodiscard]] const value_type* data_ptr() const noexcept
    requires(not std::same_as<value_type, bool>)
    {
        return this->_data.data();
    }

    /// @brief Returns a span over the segment offset array.
    /// @return A span representing the boundaries of all segments.
    ///
    /// > [!CAUTION] Use with extreme caution
    /// >
    /// > Modifying the offset values will corrupt the container's structural routing.
    [[nodiscard]] std::span<size_type> offsets_view() noexcept {
        return std::span<size_type>(this->_offsets);
    }

    /// @brief Returns a const span over the segment offset array.
    /// @return A const span representing the boundaries of all segments.
    [[nodiscard]] std::span<const size_type> offsets_view() const noexcept {
        return std::span<const size_type>(this->_offsets);
    }

    /// @brief Returns a reference to the underlying segment offset container.
    /// @return  A mutable reference to the *offsets* vector.
    ///
    /// > [!CAUTION] Use with extreme caution
    /// >
    /// > Modifying this vector directly (resizing or altering values) may corrupt the container's integrity.
    [[nodiscard]] std::vector<size_type>& offsets_storage() noexcept {
        return this->_offsets;
    }

    /// @brief Returns a const reference to the underlying segment offset container.
    /// @return A const reference to the *offsets* vector.
    [[nodiscard]] const std::vector<size_type>& offsets_storage() const noexcept {
        return this->_offsets;
    }

    /// @brief Returns a raw pointer to the underlying segment offset array.
    /// @return A raw pointer to the first element in the *offsets* array.
    ///
    /// > [!CAUTION] Use with extreme caution
    /// >
    /// > Modifying the offsets data through this function may corrupt the container's structural routing.
    [[nodiscard]] size_type* offsets_ptr() noexcept {
        return this->_offsets.data();
    }

    /// @brief Returns a const raw pointer to the underlying segment offset array.
    /// @return A const raw pointer to the first element in the *offsets* array.
    [[nodiscard]] const size_type* offsets_ptr() const noexcept {
        return this->_offsets.data();
    }

    // --- iterators ---

    /// @brief Returns a mutable iterator to the first segment.
    /// @return Iterator to the first segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] iterator begin() noexcept {
        return iterator(this->_data.begin(), this->_offsets.data());
    }

    /// @brief Returns a mutable iterator past the last segment (end sentinel).
    /// @return Iterator one position past the last segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] iterator end() noexcept {
        return iterator(this->_data.begin(), this->_offsets.data() + this->size());
    }

    /// @brief Returns a const iterator to the first segment.
    /// @return Const iterator to the first segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] const_iterator begin() const noexcept {
        return const_iterator(this->_data.begin(), this->_offsets.data());
    }

    /// @brief Returns a const iterator past the last segment (end sentinel).
    /// @return Const iterator one position past the last segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] const_iterator end() const noexcept {
        return const_iterator(this->_data.begin(), this->_offsets.data() + this->size());
    }

    /// @brief Returns a const iterator to the first segment (explicit const form).
    /// @return Const iterator to the first segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return this->begin();
    }

    /// @brief Returns a const iterator past the last segment (explicit const form).
    /// @return Const iterator one past the last segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] const_iterator cend() const noexcept {
        return this->end();
    }

    /// @brief Returns a reverse iterator to the last segment.
    /// @return Reverse iterator starting at the last segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] reverse_iterator rbegin() noexcept {
        return reverse_iterator(this->end());
    }

    /// @brief Returns a reverse iterator before the first segment (end sentinel).
    /// @return Reverse iterator one position before the first segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] reverse_iterator rend() noexcept {
        return reverse_iterator(this->begin());
    }

    /// @brief Returns a const reverse iterator to the last segment.
    /// @return Const reverse iterator starting at the last segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(this->end());
    }

    /// @brief Returns a const reverse iterator before the first segment (end sentinel).
    /// @return Const reverse iterator one position before the first segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(this->begin());
    }

    /// @brief Returns a const reverse iterator to the last segment (explicit const form).
    /// @return Const reverse iterator starting at the last segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return this->rbegin();
    }

    /// @brief Returns a const reverse iterator before the first segment (explicit const form).
    /// @return Const reverse iterator one position before the first segment.
    /// > [!NOTE] Iterator invalidated by structural modifications.
    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return this->rend();
    }

    // --- modifiers (segments) ---

    /// @brief Appends a new segment to the end of the container.
    ///
    /// This method efficiently adds a segment from any input range. If the range has a known size,
    /// appropriate pre-allocation is performed.
    ///
    /// @tparam R An input range of elements convertible to `value_type`.
    /// @param r The range to append as a new segment.
    /// ### Postconditions
    /// 1. `size()` is incremented by 1
    /// 2. `data_size()` increases by the range size.
    /// @throws std::bad_alloc If memory allocation fails.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > Amortized $O(N)$ where $N$ is the size of the pushed range.
    ///
    /// > [!NOTE] Exception safety
    /// >
    /// > Provides strong exception guarantee: if an exception occurs, the internal *offsets* and
    /// > *data* remain synchronized and the container is unchanged.
    ///
    /// > [!WARNING] Invalidates all iterators, pointers, and references to elements if reallocation occurs.
    template <std::ranges::input_range R>
    void push_back(R&& r)
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    {
        this->_ensure_offset_capacity();

        if constexpr (std::ranges::contiguous_range<R>) {
            auto* ptr = std::ranges::data(r);
            const auto n = std::ranges::size(r);
            this->_data.insert(this->_data.end(), ptr, ptr + n);
        }
        else {
            this->_data.insert(this->_data.end(), std::ranges::begin(r), std::ranges::end(r));
        }

        this->_offsets.push_back(this->_data.size());
    }

    /// @brief Appends a segment from an initializer list.
    /// @param ilist The initializer list to append as a segment.
    /// @post `size()` is incremented by 1; `data_size()` increases by the list size.
    /// @throws std::bad_alloc If memory allocation fails.
    ///
    /// > [!INFO] Implementation
    /// >
    /// > This function is equivalent to calling `push_back(std::span<const value_type>{ilist})`.
    void push_back(std::initializer_list<value_type> ilist) {
        this->push_back(std::span<const value_type>{ilist});
    }

    /// @brief Removes the last segment from the container.
    /// @post If container was not empty, `size()` is decremented by 1 and `data_size()` decreases
    ///       by the size of the removed segment. If empty, this function has no effect.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > $O(1)$ amortized for the container overhead, plus $O(N)$ to truncate
    /// > the underlying *data* vector where N is the size of the removed segment.
    ///
    /// > [!NOTE] Safety
    /// >
    /// > A call to `pop_back()` on an empty container is safe (no-op).
    ///
    /// > [!WARNING] Invalidates all iterators, pointers, and references to elements in the last segment.
    void pop_back() {
        if (this->empty())
            return;

        this->_offsets.pop_back();
        this->_data.resize(this->_offsets.back());
    }

    /// @brief Inserts a new segment at the specified position.
    ///
    /// This method inserts a range as a new segment at the given position, shifting all subsequent
    /// segments and updating their offsets accordingly.
    ///
    /// @tparam R An input range of elements convertible to `value_type`.
    /// @param pos The position where the segment will be inserted (must satisfy `pos <= size()`).
    /// @param r The range to insert as a segment.
    /// ### Postconditions
    /// 1. `size()` is incremented by 1
    /// 2. Segments at and after `pos` are shifted and offsets are updated.
    /// @throws std::bad_alloc If memory allocation fails.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > $O(N + M)$ where $N$ is the total number of elements from `pos` onward and $M$ is the size of the
    /// > inserted range, due to element shifting and offset updates. Insertion at the end is amortized $O(M)$.
    ///
    /// > [!NOTE] Exception safety
    /// >
    /// > Provides strong exception guarantee: if an exception occurs, the internal *offsets* and
    /// > *data* remain synchronized and the container is unchanged.
    ///
    /// > [!WARNING] Invalidates all iterators, pointers, and references to elements after the insertion point.
    template <std::ranges::input_range R>
    void insert(size_type pos, R&& r)
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    {
        const auto beg = this->_offsets[pos];
        const auto beg_pos = to_diff(beg);
        const auto old_size = this->_data.size();

        this->_ensure_offset_capacity();

        if constexpr (std::ranges::contiguous_range<R>) {
            auto* ptr = std::ranges::data(r);
            const auto n = std::ranges::size(r);
            this->_data.insert(this->_data.begin() + beg_pos, ptr, ptr + n);
        }
        else {
            this->_data.insert(
                this->_data.begin() + beg_pos, std::ranges::begin(r), std::ranges::end(r)
            );
        }

        const auto inserted = this->_data.size() - old_size;
        this->_offsets.insert(this->_offsets.begin() + to_diff(pos), beg);
        for (size_type i = pos + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i] += inserted;
    }

    /// @brief Inserts a segment from an initializer list at the specified position.
    /// @param pos The position where the segment will be inserted (must satisfy `pos <= size()`).
    /// @param ilist The initializer list to insert as a segment.
    /// @throws std::bad_alloc If memory allocation fails.
    ///
    /// > [!INFO] Implementation
    /// >
    /// > This function is equivalent to calling `insert(pos, std::span<const value_type>{ilist})`.
    void insert(size_type pos, std::initializer_list<value_type> ilist) {
        this->insert(pos, std::span<const value_type>{ilist});
    }

    /// @brief Erases the segment at the specified position.
    ///
    /// This method removes a segment and all subsequent segments are shifted backward,
    /// with their offsets updated accordingly.
    ///
    /// @param pos The position of the segment to erase (must satisfy `pos < size()`).
    /// ### Postconditions
    /// 1. The segment at `pos` is removed.
    /// 2. `size()` is decremented by 1.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > $O(E + S + L)$ where $E$ is the number of elements after the erased segment in the underlying
    /// > vector, $S$ is the number of segments after `pos`, and $L$ is the size of the erased segment.
    /// > Erasing the **last** segment is $O(L)$.
    ///
    /// > [!WARNING] Invalidates all iterators, pointers, and references to elements at or after the erased position.
    void erase(size_type pos) {
        const auto start = to_diff(this->_offsets[pos]);
        const auto end = to_diff(this->_offsets[pos + 1uz]);
        const auto len = static_cast<size_type>(end - start);

        this->_data.erase(this->_data.begin() + start, this->_data.begin() + end);
        this->_offsets.erase(this->_offsets.begin() + to_diff(pos));
        for (size_type i = pos; i < this->_offsets.size(); i++)
            this->_offsets[i] -= len;
    }

    // --- modifiers (elements) ---

    /// @brief Appends a copy of an element to the end of a specific segment.
    /// @param seg The segment number where the element will be appended.
    /// @param value The value to append.
    /// ### Postconditions
    /// 1. The segment size increases by 1.
    /// 2. `data_size()` increases by 1.
    /// @throws std::bad_alloc If memory allocation fails.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > Amortized $O(E + S)$ where $E$ is the number of elements after the insertion point
    /// > in the underlying vector, and $S$ is the number of segments after `seg`. Appending
    /// > to the **last** segment is amortized $O(1)$.
    ///
    /// > [!WARNING] Iterator Invalidation
    /// >
    /// > If reallocation occurs, all iterators, pointers, and references are invalidated.
    /// > Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    void push_back(size_type seg, const value_type& value) {
        this->insert(seg, this->_offsets[seg + 1uz] - this->_offsets[seg], value);
    }

    /// @brief Constructs an element in-place at the end of a specific segment.
    /// @tparam Args Perfect forwarding types for the constructor of `T`.
    /// @param seg The segment number where the element will be constructed.
    /// @param args Arguments to forward to the `T` constructor.
    /// ### Postconditions
    /// 1. The segment size increases by 1.
    /// 2. `data_size()` increases by 1.
    /// @throws Any exception thrown by the `T` constructor, or std::bad_alloc.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > Amortized $O(E + S)$ where $E$ is the number of elements after the insertion point
    /// > in the underlying vector, and $S$ is the number of segments after `seg`. Appending
    /// > to the **last** segment is amortized $O(1)$.
    ///
    /// > [!WARNING] Iterator Invalidation
    /// >
    /// > If reallocation occurs, all iterators, pointers, and references are invalidated.
    /// > Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    template <class... Args>
    void emplace_back(size_type seg, Args&&... args) {
        this->emplace(
            seg, this->_offsets[seg + 1uz] - this->_offsets[seg], std::forward<Args>(args)...
        );
    }

    /// @brief Removes the last element from a specific segment.
    /// @param seg The segment number from which to remove the last element.
    /// @post If the segment was not empty, its size decreases by 1 and `data_size()` decreases by 1.
    ///       If empty, this function has no effect.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > $O(E + S)$ where $E$ is the number of elements after the removed element in the underlying
    /// > vector, and $S$ is the number of segments after `seg`. Popping from the **last** segment is $O(1)$.
    ///
    /// > [!WARNING] Invalidates all iterators, pointers, and references to elements at or after the removed element.
    void pop_back(size_type seg) {
        const auto len = this->_offsets[seg + 1uz] - this->_offsets[seg];
        if (len == 0uz)
            return;
        this->erase(seg, len - 1uz);
    }

    /// @brief Inserts an element at a specific position within a segment.
    /// @param seg The segment number.
    /// @param pos The position within the segment where the element will be inserted.
    /// @param value value to insert.
    /// ### Postconditions
    /// 1. The segment size increases by 1.
    /// 2. `data_size()` increases by 1.
    /// @throws std::bad_alloc If memory allocation fails.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > Amortized $O(E + S)$ where $E$ is the number of elements after the insertion point
    /// > in the underlying vector, and $S$ is the number of segments after `seg`.
    ///
    /// > [!WARNING] Iterator Invalidation
    /// >
    /// > If reallocation occurs, all iterators, pointers, and references are invalidated.
    /// > Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    void insert(size_type seg, size_type pos, const value_type& value) {
        const auto insert_pos = to_diff(this->_offsets[seg] + pos);
        this->_data.insert(this->_data.begin() + insert_pos, value);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]++;
    }

    /// @brief Constructs an element in-place at a specific position within a segment.
    /// @tparam Args Perfect forwarding types for the constructor of `T`.
    /// @param seg The segment number.
    /// @param pos The position within the segment.
    /// @param args Arguments to forward to the `T` constructor.
    /// ### Postconditions
    /// 1. The segment size increases by 1.
    /// 2. `data_size()` increases by 1.
    /// @throws Any exception thrown by the `T` constructor, or std::bad_alloc.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > Amortized $O(E + S)$ where $E$ is the number of elements after the insertion point
    /// > in the underlying vector, and $S$ is the number of segments after `seg`.
    ///
    /// > [!WARNING] Iterator Invalidation
    /// >
    /// > If reallocation occurs, all iterators, pointers, and references are invalidated.
    /// > Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    template <class... Args>
    void emplace(size_type seg, size_type pos, Args&&... args) {
        const auto insert_pos = to_diff(this->_offsets[seg] + pos);
        this->_data.emplace(this->_data.begin() + insert_pos, std::forward<Args>(args)...);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]++;
    }

    /// @brief Removes an element at a specific position within a segment.
    /// @param seg The segment number.
    /// @param pos The position within the segment of the element to remove.
    /// @post The segment size decreases by 1; `data_size()` decreases by 1; offsets updated.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > $O(E + S)$ where $E$ is the number of elements after the erased position in the
    /// > underlying vector, and $S$ is the number of segments after `seg`.
    ///
    /// > [!WARNING] Invalidates all iterators, pointers, and references to elements at or after the removed position.
    void erase(size_type seg, size_type pos) {
        const auto erase_pos = to_diff(this->_offsets[seg] + pos);
        this->_data.erase(this->_data.begin() + erase_pos);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]--;
    }

    /// @brief Resizes a specific segment to contain `n` elements.
    ///
    /// - If the segment's current size is greater than `n`, it is reduced to its first `n` elements.
    /// - If the segment's current size is less than `n`, additional default-inserted elements are appended.
    /// - If the segment's current size is equal to `n`, the segment is unchanged.
    ///
    /// @param seg The segment number to resize.
    /// @param n The new size for the segment.
    /// @pre `seg < size()`
    /// @throws  std::bad_alloc If memory allocation fails during growth.
    ///
    /// > [!INFO] Implementation
    /// >
    /// > This function is equivalent to calling `resize(seg, n, value_type{})`.
    void resize(size_type seg, size_type n) {
        this->resize(seg, n, value_type());
    }

    /// @brief Resizes a specific segment to contain `n` elements, initializing new elements with `value`.
    ///
    /// - If the segment's current size is greater than `n`, it is reduced to its first `n` elements.
    /// - If the segment's current size is less than `n`, additional copies of `value` are appended.
    /// - If the segment's current size is equal to `n`, the segment is unchanged.
    ///
    /// @param seg The segment number to resize.
    /// @param n The new size for the segment.
    /// @param value The value to initialize new elements with.
    /// @pre `seg < size()`
    /// @throws std::bad_alloc If memory allocation fails during growth.
    ///
    /// > [!INFO] Time Complexity
    /// >
    /// > Amortized $O(E + S + K)$ where $E$ is the number of elements after the modification
    /// > point in the underlying vector, $S$ is the number of segments after `seg`, and $K$
    /// > is the number of elements added or removed.
    ///
    /// > [!WARNING] No bounds checking. Results in Undefined Behavior if segment index is out of bounds.
    ///
    /// > [!WARNING] Iterator Invalidation
    /// >
    /// > If reallocation occurs, all iterators, pointers, and references are invalidated.
    /// > Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    void resize(size_type seg, size_type n, const value_type& value) {
        const auto curr_count = this->segment_size(seg);
        if (n < curr_count) {
            const auto diff = curr_count - n;
            const auto start = to_diff(this->_offsets[seg] + n);
            const auto end = to_diff(this->_offsets[seg + 1uz]);

            this->_data.erase(this->_data.begin() + start, this->_data.begin() + end);
            for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
                this->_offsets[i] -= diff;
        }
        else if (n > curr_count) {
            const auto diff = n - curr_count;
            const auto pos = to_diff(this->_offsets[seg + 1uz]);

            this->_data.insert(this->_data.begin() + pos, diff, value);
            for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
                this->_offsets[i] += diff;
        }
    }

private:
    void _check_range(size_type n) const {
        if (n >= this->size())
            throw std::out_of_range(std::format(
                "flat_jagged_vector::_check_range: n (which is {}) >= this->size() (which is {})",
                n,
                this->size()
            ));
    }

    void _check_segment_range(size_type seg, size_type pos) const {
        if (pos >= this->segment_size(seg)) {
            throw std::out_of_range(std::format(
                "flat_jagged_vector::_check_segment_range: pos (which is {}) >= segment_size({}) "
                "(which is {})",
                pos,
                seg,
                this->segment_size(seg)
            ));
        }
    }

    void _ensure_offset_capacity() {
        const auto current_cap = this->_offsets.capacity();
        if (this->_offsets.size() == current_cap)
            this->_offsets.reserve(current_cap == 0uz ? 8uz : current_cap * 2uz);
    }

    std::vector<value_type> _data;
    std::vector<size_type> _offsets{0uz};
};

} // namespace gl
