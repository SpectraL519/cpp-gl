// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <initializer_list>
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>

namespace gl::types {

/// @brief A flattened 2D vector (jagged array) providing efficient storage for variable-length segments.
///
/// This container stores all elements in a single contiguous memory block (`_data`) while maintaining
/// an offsets array (`_offsets`) to track segment boundaries. This design provides excellent cache locality
/// and efficient iteration over individual segments.
///
/// @tparam T A semiregular type to be stored in the segments. Must be copy-constructible and assignable.
///
/// @note Behavior is similar to `std::vector<std::vector<T>>` but with flattened memory layout.
/// @warning Iterator invalidation follows `std::vector` semantics: modifying the structure or elements
///          invalidates all iterators, pointers, and references to the container's elements.
/// @todo Implement resize, assign, and swap methods.
/// @todo Add `operator<<` overload for `std::ostream` and specialize `std::formatter`.
template <std::semiregular T>
class segment_vector {
public:
    /// @brief Type of elements stored in segments
    using value_type = T;
    /// @brief Unsigned integral type used for sizes and indices
    using size_type = std::size_t;
    /// @brief Reference to an element
    using reference = value_type&;
    /// @brief Const reference to an element
    using const_reference = const value_type&;
    /// @brief Span type representing a non-owning segment of elements
    using segment_type = std::span<value_type>;
    /// @brief Const span type representing a non-owning const segment of elements
    using const_segment_type = std::span<const value_type>;

    // --- iterators ---

    /// @brief Random access iterator over segments of the `segment_vector`.
    ///
    /// This iterator dereferences to a `segment_type` (span of elements in a single segment),
    /// allowing efficient iteration and random access to individual segments. The iterator maintains
    /// pointers to the element data and the offsets array for dereferencing.
    ///
    /// @tparam Const If `true`, produces const iterators; if `false`, produces mutable iterators.
    /// @note Provides random access semantics: `O(1)` for all operations except construction.
    /// @warning Invalidated when the `segment_vector` is modified (structure changes or element insertions/deletions).
    template <bool Const>
    class segment_iterator {
        using data_ptr_type = std::conditional_t<Const, const T*, T*>;
        using offset_ptr_type = const size_type*;

    public:
        /// @brief Satisfies random access iterator concept
        using iterator_concept = std::random_access_iterator_tag;
        /// @brief Legacy iterator category (random access)
        using iterator_category = std::random_access_iterator_tag;
        /// @brief Type of segment this iterator dereferences to (span or const span)
        using value_type = std::conditional_t<Const, const_segment_type, segment_type>;
        /// @brief Signed integral difference type
        using difference_type = std::ptrdiff_t;
        /// @brief Pointer type (void because segment iterators dereference to spans)
        using pointer = void;
        /// @brief Reference type (span of elements)
        using reference = value_type;

        /// @brief Default constructor creates a null iterator
        segment_iterator() = default;

        /// @brief Constructs an iterator pointing to a specific segment.
        /// @param data_ptr Pointer to the underlying element data (may be null for null iterator)
        /// @param offset_ptr Pointer to the offsets array at the position of this segment
        segment_iterator(data_ptr_type data_ptr, offset_ptr_type offset_ptr) noexcept
        : _data_ptr(data_ptr), _offset_ptr(offset_ptr) {}

        /// @brief Implicit conversion from mutable to const iterator
        /// @return A const iterator pointing to the same segment
        operator segment_iterator<true>() const noexcept
        requires(not Const)
        {
            return segment_iterator<true>(this->_data_ptr, this->_offset_ptr);
        }

        /// @brief Dereferences the iterator to the current segment.
        /// @return A span representing the segment at the current position
        [[nodiscard]] reference operator*() const noexcept {
            const auto beg = *this->_offset_ptr;
            const auto end = *(this->_offset_ptr + 1uz);
            return reference(this->_data_ptr + beg, end - beg);
        }

        /// @brief Random access to a segment at offset from current position.
        /// @param n Offset (can be negative)
        /// @return Segment at offset n from the current position
        /// @pre `0 <= current_position + n < container.size()`; otherwise Undefined Behavior
        [[nodiscard]] reference operator[](difference_type n) const noexcept {
            return *(*this + n);
        }

        /// @brief Pre-increment operator.
        /// @return Reference to this iterator after advancing to the next segment
        segment_iterator& operator++() noexcept {
            ++this->_offset_ptr;
            return *this;
        }

        /// @brief Post-increment operator.
        /// @return A copy of this iterator before the increment
        segment_iterator operator++(int) noexcept {
            auto tmp = *this;
            ++this->_offset_ptr;
            return tmp;
        }

        /// @brief Pre-decrement operator.
        /// @return Reference to this iterator after moving to the previous segment
        segment_iterator& operator--() noexcept {
            --this->_offset_ptr;
            return *this;
        }

        /// @brief Post-decrement operator.
        /// @return A copy of this iterator before the decrement
        segment_iterator operator--(int) noexcept {
            auto tmp = *this;
            --this->_offset_ptr;
            return tmp;
        }

        /// @brief Advances the iterator by n positions.
        /// @param n Number of segments to advance (can be negative)
        /// @return Reference to this iterator
        segment_iterator& operator+=(difference_type n) noexcept {
            this->_offset_ptr += n;
            return *this;
        }

        /// @brief Moves the iterator backward by n positions.
        /// @param n Number of segments to move backward (can be negative)
        /// @return Reference to this iterator
        segment_iterator& operator-=(difference_type n) noexcept {
            this->_offset_ptr -= n;
            return *this;
        }

        /// @brief Creates a new iterator advanced by n positions from the given iterator.
        /// @param it Iterator to advance from
        /// @param n Number of segments to advance
        /// @return New iterator at the advanced position
        [[nodiscard]] friend segment_iterator operator+(
            segment_iterator it, difference_type n
        ) noexcept {
            return it += n;
        }

        /// @brief Creates a new iterator advanced by n positions (commutative form).
        /// @param n Number of segments to advance
        /// @param it Iterator to advance from
        /// @return New iterator at the advanced position
        [[nodiscard]] friend segment_iterator operator+(
            difference_type n, segment_iterator it
        ) noexcept {
            return it += n;
        }

        /// @brief Creates a new iterator moved backward by n positions.
        /// @param it Iterator to move backward from
        /// @param n Number of segments to move backward
        /// @return New iterator at the moved position
        [[nodiscard]] friend segment_iterator operator-(
            segment_iterator it, difference_type n
        ) noexcept {
            return it -= n;
        }

        /// @brief Computes the distance between two iterators.
        /// @param lhs The later iterator
        /// @param rhs The earlier iterator
        /// @return Number of segments between the iterators; negative if lhs < rhs
        [[nodiscard]] friend difference_type operator-(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr - rhs._offset_ptr;
        }

        /// @brief Tests equality of two iterators.
        /// @param lhs Left iterator
        /// @param rhs Right iterator
        /// @return `true` if both iterators point to the same segment
        [[nodiscard]] friend bool operator==(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr == rhs._offset_ptr;
        }

        /// @brief Three-way comparison of two iterators.
        /// @param lhs Left iterator
        /// @param rhs Right iterator
        /// @return Comparison result indicating iterator ordering
        [[nodiscard]] friend auto operator<=>(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr <=> rhs._offset_ptr;
        }

    private:
        data_ptr_type _data_ptr{nullptr};
        offset_ptr_type _offset_ptr{nullptr};
    };

    /// @brief Mutable random access iterator over segments
    using iterator = segment_iterator<false>;
    /// @brief Const random access iterator over segments
    using const_iterator = segment_iterator<true>;
    /// @brief Reverse mutable iterator
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// @brief Reverse const iterator
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // --- constructors ---

    /// @brief Default constructor creates an empty `segment_vector`.
    /// @post `empty() == true`, `size() == 0`, `data_size() == 0`
    segment_vector() = default;

    /// @brief Copy constructor creates a deep copy of another `segment_vector`.
    /// @param other The `segment_vector` to copy
    /// @post `*this == other`
    segment_vector(const segment_vector&) = default;
    /// @brief Copy assignment creates a deep copy of another `segment_vector`.
    /// @param other The source `segment_vector`
    /// @return Reference to `*this`
    /// @post `*this == other`
    segment_vector& operator=(const segment_vector&) = default;

    /// @brief Move constructor transfers ownership of data from another `segment_vector`.
    /// @param other The source `segment_vector` (left in a valid but unspecified state)
    /// @post `other.empty() == true`; all data is transferred to `*this`
    /// @warning Invalidates all iterators, pointers, and references to `other`'s elements
    segment_vector(segment_vector&& other) noexcept
    : _data(std::move(other._data)), _offsets(std::move(other._offsets)) {
        other._offsets = {0uz};
    }

    /// @brief Move assignment transfers ownership of data from another `segment_vector`.
    /// @param other The source `segment_vector`
    /// @return Reference to `*this`
    /// @post `other.empty() == true`; all data from `other` is transferred to `*this`
    /// @warning Invalidates all iterators, pointers, and references to this container's elements.
    ///          This function safely handles self-assignment.
    segment_vector& operator=(segment_vector&& other) noexcept {
        if (this != &other) {
            this->_data = std::move(other._data);
            this->_offsets = std::move(other._offsets);
            other._offsets = {0uz};
        }
        return *this;
    }

    /// @brief Destructor cleans up all managed memory.
    ~segment_vector() = default;

    /// @brief Constructs a `segment_vector` with a specified number of segments and initial segment size.
    /// @param n_segments The number of segments to create
    /// @param segment_size The initial size of each segment (default is 0)
    /// @post `size() == n_segments` and each segment is initialized with `segment_size` default-constructed elements
    /// @exception std::bad_alloc May throw if memory allocation fails
    segment_vector(size_type n_segments, size_type segment_size = 0uz)
    : _data(n_segments * segment_size), _offsets(n_segments + 1uz) {
        for (size_type i = 0uz; i <= n_segments; ++i)
            this->_offsets[i] = i * segment_size;
    }

    /// @brief Constructs a `segment_vector` from an initializer list of segments.
    /// @param ilist Initializer list of initializer lists, each representing a segment
    /// @post `size() == ilist.size()` and `data_size()` equals the sum of all segment sizes
    /// @exception std::bad_alloc May throw if memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references after construction
    segment_vector(std::initializer_list<std::initializer_list<value_type>> ilist) {
        this->reserve_segments(ilist.size());

        size_type total_size = 0uz;
        for (const auto& sub : ilist)
            total_size += sub.size();
        this->reserve_data(total_size);

        for (const auto& sub : ilist)
            this->push_back(sub);
    }

    /// @brief Constructs a `segment_vector` from a range of ranges.
    ///
    /// This constructor accepts any input range of input ranges convertible to `value_type`,
    /// enabling flexible initialization from various container types.
    ///
    /// @tparam R A range type whose elements are input ranges of `value_type`
    /// @param r The range of ranges to initialize from
    /// @post `size()` equals the number of outer range elements; `data_size()` is the sum of all element counts
    /// @exception std::bad_alloc May throw if memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references after construction
    template <std::ranges::input_range R>
    requires std::ranges::input_range<std::ranges::range_reference_t<R>>
         and std::convertible_to<
                 std::ranges::range_reference_t<std::ranges::range_reference_t<R>>,
                 value_type>
    explicit segment_vector(R&& r) {
        if constexpr (std::ranges::sized_range<R>)
            this->reserve_segments(std::ranges::size(r));

        for (auto&& subrange : r)
            this->push_back(std::forward<decltype(subrange)>(subrange));
    }

    // --- comparsion ---

    /// @brief Tests equality of two `segment_vector` instances.
    /// @param lhs Left operand
    /// @param rhs Right operand
    /// @return `true` if both vectors have the same structure and elements
    friend bool operator==(const segment_vector&, const segment_vector&) = default;

    // --- capacity ---

    /// @brief Returns the number of segments in this container.
    /// @return The count of segments
    [[nodiscard]] size_type size() const noexcept {
        return this->_offsets.size() - 1uz;
    }

    /// @brief Checks if the container is empty (contains no segments).
    /// @return `true` if `size() == 0`, `false` otherwise
    [[nodiscard]] bool empty() const noexcept {
        return this->size() == 0uz;
    }

    /// @brief Returns the current capacity for segments (number of segment slots allocated).
    /// @return The number of segments that can be stored without reallocation
    [[nodiscard]] size_type segments_capacity() const noexcept {
        return this->_offsets.capacity() - 1uz;
    }

    /// @brief Returns the current capacity for data elements.
    /// @return The number of elements that can be stored in `_data` without reallocation
    [[nodiscard]] size_type data_capacity() const noexcept {
        return this->_data.capacity();
    }

    /// @brief Reserves space for at least n additional segments without changing the size.
    /// @param n The number of segments to reserve space for
    /// @post `segments_capacity() >= n + size()`
    /// @note This is an optimization hint; the container may allocate more than requested
    /// @warning Invalidates all iterators and pointers to elements if reallocation occurs
    void reserve_segments(size_type n) {
        this->_offsets.reserve(n + 1uz);
    }

    /// @brief Reserves space for at least n additional data elements without changing the size.
    /// @param n The number of elements to reserve space for
    /// @post `data_capacity() >= n + data_size()`
    /// @note This is an optimization hint; the container may allocate more than requested
    /// @warning Invalidates all iterators and pointers to elements if reallocation occurs
    void reserve_data(size_type n) {
        this->_data.reserve(n);
    }

    /// @brief Reduces capacity of both internal arrays to match current size.
    /// @post `segments_capacity() == size()` and `data_capacity() == data_size()`
    /// @warning Invalidates all iterators, pointers, and references to elements
    void shrink_to_fit() {
        this->_data.shrink_to_fit();
        this->_offsets.shrink_to_fit();
    }

    /// @brief Removes all segments and elements, leaving the container empty.
    /// @post `size() == 0`, `data_size() == 0`, but capacity is unchanged
    /// @warning Invalidates all iterators, pointers, and references to elements
    void clear() {
        this->_data.clear();
        this->_offsets.clear();
        this->_offsets.push_back(0uz);
    }

    // --- accessors ---

    /// @brief Returns the segment at the given index without bounds checking.
    /// @param i The index of the segment to access
    /// @return A span representing the segment at index i
    /// @pre `i < size()`; otherwise Undefined Behavior
    /// @warning No bounds checking is performed for performance. Use `at()` for bounds-checked access.
    ///          Calling on an out-of-bounds index results in Undefined Behavior.
    [[nodiscard]] segment_type operator[](size_type i) {
        const auto beg = this->_offsets[i];
        const auto end = this->_offsets[i + 1uz];
        return segment_type(this->_data.data() + beg, end - beg);
    }

    /// @brief Returns a const segment at the given index without bounds checking.
    /// @param i The index of the segment to access
    /// @return A const span representing the segment at index i
    /// @pre `i < size()`; otherwise Undefined Behavior
    /// @warning No bounds checking is performed for performance. Use `at()` for bounds-checked access.
    ///          Calling on an out-of-bounds index results in Undefined Behavior.
    [[nodiscard]] const_segment_type operator[](size_type i) const {
        const auto beg = this->_offsets[i];
        const auto end = this->_offsets[i + 1uz];
        return const_segment_type(this->_data.data() + beg, end - beg);
    }

    /// @brief Returns a reference to an element within a segment without bounds checking.
    /// @param seg The segment number
    /// @param pos The position within the segment
    /// @return Reference to the element at the given segment and position
    /// @pre `seg < size()` and `pos < segment_size(seg)`; otherwise Undefined Behavior
    /// @warning No bounds checking is performed. Use `at(seg, pos)` for bounds-checked access.
    ///          Out-of-bounds access results in Undefined Behavior.
    [[nodiscard]] reference operator[](size_type seg, size_type pos) {
        return this->_data[this->_offsets[seg] + pos];
    }

    /// @brief Returns a const reference to an element within a segment without bounds checking.
    /// @param seg The segment number
    /// @param pos The position within the segment
    /// @return Const reference to the element at the given segment and position
    /// @pre `seg < size()` and `pos < segment_size(seg)`; otherwise Undefined Behavior
    /// @warning No bounds checking is performed. Use `at(seg, pos)` for bounds-checked access.
    ///          Out-of-bounds access results in Undefined Behavior.
    [[nodiscard]] const_reference operator[](size_type seg, size_type pos) const {
        return this->_data[this->_offsets[seg] + pos];
    }

    /// @brief Returns the segment at the given index with bounds checking.
    /// @param i The index of the segment
    /// @return A span representing the segment at index i
    /// @exception std::out_of_range If `i >= size()`
    /// @note Provides the same safety as `std::vector::at()`
    [[nodiscard]] segment_type at(size_type i) {
        this->_check_range(i);
        return (*this)[i];
    }

    /// @brief Returns a const segment at the given index with bounds checking.
    /// @param i The index of the segment
    /// @return A const span representing the segment at index i
    /// @exception std::out_of_range If `i >= size()`
    /// @note Provides the same safety as `std::vector::at()`
    [[nodiscard]] const_segment_type at(size_type i) const {
        this->_check_range(i);
        return (*this)[i];
    }

    /// @brief Returns a reference to an element within a segment with bounds checking.
    /// @param seg The segment number
    /// @param pos The position within the segment
    /// @return Reference to the element at the given segment and position
    /// @exception std::out_of_range If `seg >= size()` or `pos >= segment_size(seg)`
    /// @note Provides safety similar to `std::vector::at()`
    [[nodiscard]] reference at(size_type seg, size_type pos) {
        this->_check_range(seg);
        this->_check_segment_range(seg, pos);
        return (*this)[seg, pos];
    }

    /// @brief Returns a const reference to an element within a segment with bounds checking.
    /// @param seg The segment number
    /// @param pos The position within the segment
    /// @return Const reference to the element at the given segment and position
    /// @exception std::out_of_range If `seg >= size()` or `pos >= segment_size(seg)`
    /// @note Provides safety similar to `std::vector::at()`
    [[nodiscard]] const_reference at(size_type seg, size_type pos) const {
        this->_check_range(seg);
        this->_check_segment_range(seg, pos);
        return (*this)[seg, pos];
    }

    /// @brief Returns the first segment without bounds checking.
    /// @return A span representing the first segment
    /// @pre Container must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking. Results in Undefined Behavior if container is empty.
    [[nodiscard]] segment_type front() noexcept {
        return (*this)[0uz];
    }

    /// @brief Returns a const reference to the first segment without bounds checking.
    /// @return A const span representing the first segment
    /// @pre Container must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking. Results in Undefined Behavior if container is empty.
    [[nodiscard]] const_segment_type front() const noexcept {
        return (*this)[0uz];
    }

    /// @brief Returns the last segment without bounds checking.
    /// @return A span representing the last segment
    /// @pre Container must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking. Results in Undefined Behavior if container is empty.
    [[nodiscard]] segment_type back() noexcept {
        return (*this)[this->size() - 1uz];
    }

    /// @brief Returns a const reference to the last segment without bounds checking.
    /// @return A const span representing the last segment
    /// @pre Container must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking. Results in Undefined Behavior if container is empty.
    [[nodiscard]] const_segment_type back() const noexcept {
        return (*this)[this->size() - 1uz];
    }

    /// @brief Returns a reference to the first element in a segment without bounds checking.
    /// @param seg The segment number
    /// @return Reference to the first element in the segment
    /// @pre `seg < size()` and the segment must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking. Use `at(seg, 0)` for bounds-checked access.
    [[nodiscard]] reference front(size_type seg) noexcept {
        return (*this)[seg, 0uz];
    }

    /// @brief Returns a const reference to the first element in a segment without bounds checking.
    /// @param seg The segment number
    /// @return Const reference to the first element in the segment
    /// @pre `seg < size()` and the segment must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking. Use `at(seg, 0)` for bounds-checked access.
    [[nodiscard]] const_reference front(size_type seg) const noexcept {
        return (*this)[seg, 0uz];
    }

    /// @brief Returns a reference to the last element in a segment without bounds checking.
    /// @param seg The segment number
    /// @return Reference to the last element in the segment
    /// @pre `seg < size()` and the segment must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking. Use `at(seg, segment_size(seg) - 1)` for bounds-checked access.
    [[nodiscard]] reference back(size_type seg) noexcept {
        return (*this)[seg, this->segment_size(seg) - 1uz];
    }

    /// @brief Returns a const reference to the last element in a segment without bounds checking.
    /// @param seg The segment number
    /// @return Const reference to the last element in the segment
    /// @pre `seg < size()` and the segment must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking. Use `at(seg, segment_size(seg) - 1)` for bounds-checked access.
    [[nodiscard]] const_reference back(size_type seg) const noexcept {
        return (*this)[seg, this->segment_size(seg) - 1uz];
    }

    /// @brief Returns a view of all segments for iteration.
    /// @return A range adaptable range view of all segments
    /// @note This creates a lazy view; iterating yields segments as spans
    [[nodiscard]] auto segments() noexcept {
        return std::views::iota(size_type{0}, this->size())
             | std::views::transform([this](size_type i) -> segment_type { return (*this)[i]; });
    }

    /// @brief Returns a const view of all segments for iteration.
    /// @return A const range adaptable range view of all segments
    /// @note This creates a lazy view; iterating yields const segments as const spans
    [[nodiscard]] auto segments() const noexcept {
        return std::views::iota(size_type{0}, this->size())
             | std::views::transform([this](size_type i) -> const_segment_type {
                   return (*this)[i];
               });
    }

    /// @brief Returns the number of elements in a specific segment without bounds checking.
    /// @param seg The segment number
    /// @return The count of elements in the segment
    /// @pre `seg < size()`; otherwise Undefined Behavior
    /// @warning No bounds checking. Results in Undefined Behavior if segment index is out of bounds.
    [[nodiscard]] size_type segment_size(size_type seg) const noexcept {
        return this->_offsets[seg + 1uz] - this->_offsets[seg];
    }

    /// @brief Returns the total number of elements across all segments.
    /// @return The sum of sizes of all segments
    [[nodiscard]] size_type data_size() const noexcept {
        return this->_data.size();
    }

    /// @brief Returns a span over all element data in flattened form.
    /// @return A span of all elements in the underlying `_data` array
    /// @note Allows direct access to the flattened representation of all segments
    [[nodiscard]] segment_type data() noexcept {
        return segment_type(this->_data);
    }

    /// @brief Returns a const span over all element data in flattened form.
    /// @return A const span of all elements in the underlying `_data` array
    /// @note Allows direct access to the flattened representation of all segments
    [[nodiscard]] const_segment_type data() const noexcept {
        return const_segment_type(this->_data);
    }

    // --- iterators ---

    /// @brief Returns a mutable iterator to the first segment.
    /// @return Iterator to the first segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] iterator begin() noexcept {
        return iterator(this->_data.data(), this->_offsets.data());
    }

    /// @brief Returns a mutable iterator past the last segment (end sentinel).
    /// @return Iterator one position past the last segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] iterator end() noexcept {
        return iterator(this->_data.data(), this->_offsets.data() + this->size());
    }

    /// @brief Returns a const iterator to the first segment.
    /// @return Const iterator to the first segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_iterator begin() const noexcept {
        return const_iterator(this->_data.data(), this->_offsets.data());
    }

    /// @brief Returns a const iterator past the last segment (end sentinel).
    /// @return Const iterator one position past the last segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_iterator end() const noexcept {
        return const_iterator(this->_data.data(), this->_offsets.data() + this->size());
    }

    /// @brief Returns a const iterator to the first segment (explicit const form).
    /// @return Const iterator to the first segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return this->begin();
    }

    /// @brief Returns a const iterator past the last segment (explicit const form).
    /// @return Const iterator one past the last segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_iterator cend() const noexcept {
        return this->end();
    }

    /// @brief Returns a reverse iterator to the last segment.
    /// @return Reverse iterator starting at the last segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] reverse_iterator rbegin() noexcept {
        return reverse_iterator(this->end());
    }

    /// @brief Returns a reverse iterator before the first segment (end sentinel).
    /// @return Reverse iterator one position before the first segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] reverse_iterator rend() noexcept {
        return reverse_iterator(this->begin());
    }

    /// @brief Returns a const reverse iterator to the last segment.
    /// @return Const reverse iterator starting at the last segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(this->end());
    }

    /// @brief Returns a const reverse iterator before the first segment (end sentinel).
    /// @return Const reverse iterator one position before the first segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(this->begin());
    }

    /// @brief Returns a const reverse iterator to the last segment (explicit const form).
    /// @return Const reverse iterator starting at the last segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return this->rbegin();
    }

    /// @brief Returns a const reverse iterator before the first segment (explicit const form).
    /// @return Const reverse iterator one position before the first segment
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return this->rend();
    }

    // --- modifiers (segments) ---

    /// @brief Appends a new segment to the end of the container.
    ///
    /// This method efficiently adds a segment from any input range. If the range has a known size,
    /// appropriate pre-allocation is performed.
    ///
    /// @tparam R An input range of elements convertible to `value_type`
    /// @param r The range to append as a new segment
    /// @post `size()` is incremented by 1; `data_size()` increases by the range size
    /// @exception std::bad_alloc If memory allocation fails
    /// @note Provides strong exception guarantee: if an exception occurs, the internal `_offsets`
    ///       and `_data` remain synchronized and the container is unchanged.
    /// @warning Invalidates all iterators, pointers, and references to elements if reallocation occurs.
    ///          Time complexity is amortized $O(N)$ where N is the size of the range.
    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void push_back(R&& r) {
        this->_ensure_offset_capacity();
        if constexpr (std::ranges::sized_range<R>)
            this->_data.reserve(this->_data.size() + std::ranges::size(r));

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
    /// @param ilist The initializer list to append as a segment
    /// @post `size()` is incremented by 1; `data_size()` increases by the list size
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references to elements if reallocation occurs.
    ///          Time complexity is amortized $O(N)$ where N is the number of elements in ilist.
    void push_back(std::initializer_list<value_type> ilist) {
        this->push_back(std::span<const value_type>{ilist});
    }

    /// @brief Removes the last segment from the container.
    /// @post If container was not empty, `size()` is decremented by 1 and `data_size()` decreases
    ///       by the size of the removed segment. If empty, this function has no effect.
    /// @note A call to `pop_back()` on an empty container is safe (no-op).
    /// @warning Invalidates all iterators, pointers, and references to elements in the last segment.
    ///          Time complexity is $O(1)$ amortized for the container overhead, plus $O(N)$ to truncate
    ///          the underlying `_data` vector where N is the size of the removed segment.
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
    /// @tparam R An input range of elements convertible to `value_type`
    /// @param pos The position where the segment will be inserted (must satisfy `pos <= size()`)
    /// @param r The range to insert as a segment
    /// @post `size()` is incremented by 1; segments at and after `pos` are shifted; offsets updated
    /// @exception std::bad_alloc If memory allocation fails
    /// @note Provides strong exception guarantee regarding `_offsets` and `_data` synchronization.
    /// @warning Invalidates all iterators, pointers, and references to elements after the insertion point.
    ///          **Time complexity is $O(N + M)$** where N is the total number of elements from `pos` onward
    ///          and M is the size of the inserted range, due to element shifting and offset updates.
    ///          Insertion at the end is amortized $O(M)$.
    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void insert(size_type pos, R&& r) {
        const auto beg = this->_offsets[pos];
        const auto old_size = this->_data.size();

        this->_ensure_offset_capacity();
        if constexpr (std::ranges::sized_range<R>)
            this->_data.reserve(this->_data.size() + std::ranges::size(r));

        if constexpr (std::ranges::contiguous_range<R>) {
            auto* ptr = std::ranges::data(r);
            const auto n = std::ranges::size(r);
            this->_data.insert(this->_data.begin() + beg, ptr, ptr + n);
        }
        else {
            this->_data.insert(
                this->_data.begin() + beg, std::ranges::begin(r), std::ranges::end(r)
            );
        }

        const auto inserted = this->_data.size() - old_size;
        this->_offsets.insert(this->_offsets.begin() + pos, beg);
        for (size_type i = pos + 1uz; i < this->_offsets.size(); ++i)
            this->_offsets[i] += inserted;
    }

    /// @brief Inserts a segment from an initializer list at the specified position.
    /// @param pos The position where the segment will be inserted (must satisfy `pos <= size()`)
    /// @param ilist The initializer list to insert as a segment
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning If reallocation occurs, all iterators, pointers, and references are invalidated.
    ///          Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    /// @note **Time Complexity:** Amortized $O(E + S + L)$ where $E$ is the number of elements
    ///       after the insertion point in the underlying vector, $S$ is the number of segments
    ///       after `pos`, and $L$ is the number of elements in `ilist`. Insertion at the end is amortized $O(L)$.
    void insert(size_type pos, std::initializer_list<value_type> ilist) {
        this->insert(pos, std::span<const value_type>{ilist});
    }

    /// @brief Erases the segment at the specified position.
    ///
    /// This method removes a segment and all subsequent segments are shifted backward,
    /// with their offsets updated accordingly.
    ///
    /// @param pos The position of the segment to erase (must satisfy `pos < size()`)
    /// @post The segment at `pos` is removed; `size()` is decremented by 1
    /// @warning Invalidates all iterators, pointers, and references to elements at or after the erased position.
    /// @note **Time Complexity:** $O(E + S + L)$ where $E$ is the number of elements after
    ///       the erased segment in the underlying vector, $S$ is the number of segments after `pos`,
    ///       and $L$ is the size of the erased segment. Erasing the **last** segment is $O(L)$.
    void erase(size_type pos) {
        const auto start = this->_offsets[pos];
        const auto end = this->_offsets[pos + 1uz];
        const auto len = end - start;

        this->_data.erase(this->_data.begin() + start, this->_data.begin() + end);
        this->_offsets.erase(this->_offsets.begin() + pos);
        for (size_type i = pos; i < this->_offsets.size(); i++)
            this->_offsets[i] -= len;
    }

    // --- modifiers (elements) ---

    /// @brief Appends a copy of an element to the end of a specific segment.
    /// @param seg The segment number where the element will be appended
    /// @param value The value to append
    /// @post The segment size increases by 1; `data_size()` increases by 1
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning If reallocation occurs, all iterators, pointers, and references are invalidated.
    ///          Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    /// @note **Time Complexity:** Amortized $O(E + S)$ where $E$ is the number of elements
    ///       after the insertion point in the underlying vector, and $S$ is the number of segments
    ///       after `seg`. Appending to the **last** segment is amortized $O(1)$.
    void push_back(size_type seg, const value_type& value) {
        this->insert(seg, this->_offsets[seg + 1uz] - this->_offsets[seg], value);
    }

    /// @brief Constructs an element in-place at the end of a specific segment.
    /// @tparam Args Perfect forwarding types for the constructor of `T`
    /// @param seg The segment number where the element will be constructed
    /// @param args Arguments to forward to the `T` constructor
    /// @post The segment size and `data_size()` increase by 1
    /// @exception Any exception thrown by the `T` constructor, or std::bad_alloc
    /// @warning If reallocation occurs, all iterators, pointers, and references are invalidated.
    ///          Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    /// @note **Time Complexity:** Amortized $O(E + S)$ where $E$ is the number of elements
    ///       after the insertion point in the underlying vector, and $S$ is the number of segments
    ///       after `seg`. Appending to the **last** segment is amortized $O(1)$.
    template <class... Args>
    void emplace_back(size_type seg, Args&&... args) {
        this->emplace(
            seg, this->_offsets[seg + 1uz] - this->_offsets[seg], std::forward<Args>(args)...
        );
    }

    /// @brief Removes the last element from a specific segment.
    /// @param seg The segment number from which to remove the last element
    /// @post If the segment was not empty, its size decreases by 1 and `data_size()` decreases by 1.
    ///       If empty, this function has no effect.
    /// @warning Invalidates all iterators, pointers, and references to elements at or after the removed element.
    /// @note **Time Complexity:** $O(E + S)$ where $E$ is the number of elements after the removed
    ///       element in the underlying vector, and $S$ is the number of segments after `seg`.
    ///       Popping from the **last** segment is $O(1)$.
    void pop_back(size_type seg) {
        const auto len = this->_offsets[seg + 1uz] - this->_offsets[seg];
        if (len == 0uz)
            return;
        this->erase(seg, len - 1uz);
    }

    /// @brief Inserts an element at a specific position within a segment.
    /// @param seg The segment number
    /// @param pos The position within the segment where the element will be inserted
    /// @param value The value to insert
    /// @post The segment size increases by 1; `data_size()` increases by 1; offsets updated
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning If reallocation occurs, all iterators, pointers, and references are invalidated.
    ///          Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    /// @note **Time Complexity:** Amortized $O(E + S)$ where $E$ is the number of elements after
    ///       the insertion point in the underlying vector, and $S$ is the number of segments after `seg`.
    void insert(size_type seg, size_type pos, const value_type& value) {
        this->_data.insert(this->_data.begin() + this->_offsets[seg] + pos, value);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]++;
    }

    /// @brief Constructs an element in-place at a specific position within a segment.
    /// @tparam Args Perfect forwarding types for the constructor of `T`
    /// @param seg The segment number
    /// @param pos The position within the segment
    /// @param args Arguments to forward to the `T` constructor
    /// @post The segment size increases by 1; `data_size()` increases by 1; offsets updated
    /// @exception Any exception thrown by the `T` constructor, or std::bad_alloc
    /// @warning If reallocation occurs, all iterators, pointers, and references are invalidated.
    ///          Otherwise, only those pointing to elements at or after the insertion point are invalidated.
    /// @note **Time Complexity:** Amortized $O(E + S)$ where $E$ is the number of elements after
    ///       the insertion point in the underlying vector, and $S$ is the number of segments after `seg`.
    template <class... Args>
    void emplace(size_type seg, size_type pos, Args&&... args) {
        this->_data.emplace(
            this->_data.begin() + this->_offsets[seg] + pos, std::forward<Args>(args)...
        );
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]++;
    }

    /// @brief Removes an element at a specific position within a segment.
    /// @param seg The segment number
    /// @param pos The position within the segment of the element to remove
    /// @post The segment size decreases by 1; `data_size()` decreases by 1; offsets updated
    /// @warning Invalidates all iterators, pointers, and references to elements at or after the removed position.
    /// @note **Time Complexity:** $O(E + S)$ where $E$ is the number of elements after the erased
    ///       position in the underlying vector, and $S$ is the number of segments after `seg`.
    void erase(size_type seg, size_type pos) {
        this->_data.erase(this->_data.begin() + this->_offsets[seg] + pos);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]--;
    }

private:
    /// @brief Validates that the segment index is within bounds.
    /// @param n The segment index to check
    /// @exception std::out_of_range If `n >= size()`
    /// @note Used internally by `at()` methods to provide bounds checking
    void _check_range(size_type n) const {
        if (n >= this->size())
            throw std::out_of_range(std::format(
                "segment_vector::_check_range: n (which is {}) >= this->size() (which is {})",
                n,
                this->size()
            ));
    }

    /// @brief Validates that an element position is within a segment's bounds.
    /// @param seg The segment number (assumed valid)
    /// @param pos The position within the segment to check
    /// @exception std::out_of_range If `pos >= segment_size(seg)`
    /// @note Used internally by `at(seg, pos)` methods to provide bounds checking
    void _check_segment_range(size_type seg, size_type pos) const {
        if (pos >= this->segment_size(seg)) {
            throw std::out_of_range(std::format(
                "segment_vector::_check_segment_range: pos (which is {}) >= segment_size({}) "
                "(which is {})",
                pos,
                seg,
                this->segment_size(seg)
            ));
        }
    }

    /// @brief Ensures sufficient capacity in the offsets array for the next segment.
    /// @note Used internally before adding a segment to guarantee space without throwing
    ///       (provides exponential growth strategy: 8 initially, then 2x current capacity)
    void _ensure_offset_capacity() {
        const auto current_cap = this->_offsets.capacity();
        if (this->_offsets.size() == current_cap)
            this->_offsets.reserve(current_cap == 0uz ? 8uz : current_cap * 2uz);
    }

    std::vector<value_type> _data;
    std::vector<size_type> _offsets{0uz};
};

} // namespace gl::types
