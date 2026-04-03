// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <initializer_list>
#include <ranges>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

namespace gl {

/// @brief A flattened 2D matrix providing efficient storage and uniform access for a rectangular grid of elements.
///
/// This container stores all elements in a single contiguous memory block (`_data`) of size `n_rows * n_cols`
/// using row-major ordering. Row accesses are contiguous in memory, while column accesses are resolved mathematically
/// via strided views. Both provide $O(1)$ random access and native compatibility with C++20/23 ranges.
///
/// @tparam T A semiregular type to be stored in the matrix. Must be copy-constructible and assignable.
///
/// @warning Iterator invalidation follows `std::vector` semantics: modifying the dimensions or structural
///          capacity of the matrix invalidates all iterators, pointers, and references to its elements.
template <std::semiregular T>
class flat_matrix {
public:
    /// @brief Type of elements stored in the matrix
    using value_type = T;
    /// @brief Unsigned integral type used for sizes and indices
    using size_type = std::size_t;
    /// @brief Reference to an element
    using reference = value_type&;
    /// @brief Const reference to an element
    using const_reference = const value_type&;
    /// @brief Span type representing a non-owning uniform row of elements
    using row_type = std::span<value_type>;
    /// @brief Const span type representing a non-owning uniform const row of elements
    using const_row_type = std::span<const value_type>;

    // --- iterators ---

    /// @brief Random access iterator over the rows of the `flat_matrix`.
    ///
    /// This iterator dereferences to a `row_type` (span of elements representing a single matrix row),
    /// allowing efficient iteration and random access. It calculates the memory offsets mathematically
    /// based on the column dimension.
    ///
    /// @tparam Const If `true`, produces const iterators; if `false`, produces mutable iterators.
    /// @note Provides random access semantics: `O(1)` for all operations except construction.
    /// @warning Invalidated when the `flat_matrix` structural dimensions are modified or memory is reallocated.
    template <bool Const>
    class row_iterator {
        using data_ptr_type = std::conditional_t<Const, const T*, T*>;

    public:
        /// @brief Satisfies random access iterator concept
        using iterator_concept = std::random_access_iterator_tag;
        /// @brief Legacy iterator category (random access)
        using iterator_category = std::random_access_iterator_tag;
        /// @brief Type of row this iterator dereferences to (span or const span)
        using value_type = std::conditional_t<Const, const_row_type, row_type>;
        /// @brief Signed integral difference type
        using difference_type = std::ptrdiff_t;
        /// @brief Pointer type (void because iterators dereference to spans)
        using pointer = void;
        /// @brief Reference type (span of elements)
        using reference = value_type;

        /// @brief Default constructor creates a null iterator
        row_iterator() = default;

        /// @brief Constructs an iterator pointing to a specific row.
        /// @param data_ptr Pointer to the underlying flat element data
        /// @param n_cols The number of columns in the matrix
        /// @param row_idx The index of the row this iterator currently points to
        row_iterator(data_ptr_type data_ptr, size_type n_cols, size_type row_idx) noexcept
        : _data_ptr(data_ptr), _row_size(n_cols), _row_idx(row_idx) {}

        /// @brief Implicit conversion from mutable to const iterator
        /// @return A const iterator pointing to the same row
        operator row_iterator<true>() const noexcept
        requires(not Const)
        {
            return row_iterator<true>(this->_data_ptr, this->_row_size, this->_row_idx);
        }

        /// @brief Dereferences the iterator to the current row.
        /// @return A span representing the row at the current position
        [[nodiscard]] reference operator*() const noexcept {
            return reference(this->_data_ptr + this->_row_idx * this->_row_size, this->_row_size);
        }

        /// @brief Random access to a row at an offset from the current position.
        /// @param n Offset (can be negative)
        /// @return Row at offset n from the current position
        /// @pre `0 <= current_position + n < n_rows()`; otherwise Undefined Behavior
        [[nodiscard]] reference operator[](difference_type n) const noexcept {
            return *(*this + n);
        }

        /// @brief Pre-increment operator.
        /// @return Reference to this iterator after advancing to the next row
        row_iterator& operator++() noexcept {
            ++this->_row_idx;
            return *this;
        }

        /// @brief Post-increment operator.
        /// @return A copy of this iterator before the increment
        row_iterator operator++(int) noexcept {
            auto tmp = *this;
            ++this->_row_idx;
            return tmp;
        }

        /// @brief Pre-decrement operator.
        /// @return Reference to this iterator after moving to the previous row
        row_iterator& operator--() noexcept {
            --this->_row_idx;
            return *this;
        }

        /// @brief Post-decrement operator.
        /// @return A copy of this iterator before the decrement
        row_iterator operator--(int) noexcept {
            auto tmp = *this;
            --this->_row_idx;
            return tmp;
        }

        /// @brief Advances the iterator by n rows.
        /// @param n Number of rows to advance (can be negative)
        /// @return Reference to this iterator
        row_iterator& operator+=(difference_type n) noexcept {
            this->_row_idx += static_cast<size_type>(n);
            return *this;
        }

        /// @brief Moves the iterator backward by n rows.
        /// @param n Number of rows to move backward (can be negative)
        /// @return Reference to this iterator
        row_iterator& operator-=(difference_type n) noexcept {
            this->_row_idx -= static_cast<size_type>(n);
            return *this;
        }

        /// @brief Creates a new iterator advanced by n rows from the given iterator.
        /// @param it Iterator to advance from
        /// @param n Number of rows to advance
        /// @return New iterator at the advanced position
        [[nodiscard]] friend row_iterator operator+(row_iterator it, difference_type n) noexcept {
            return it += n;
        }

        /// @brief Creates a new iterator advanced by n rows (commutative form).
        /// @param n Number of rows to advance
        /// @param it Iterator to advance from
        /// @return New iterator at the advanced position
        [[nodiscard]] friend row_iterator operator+(difference_type n, row_iterator it) noexcept {
            return it += n;
        }

        /// @brief Creates a new iterator moved backward by n rows.
        /// @param it Iterator to move backward from
        /// @param n Number of rows to move backward
        /// @return New iterator at the moved position
        [[nodiscard]] friend row_iterator operator-(row_iterator it, difference_type n) noexcept {
            return it -= n;
        }

        /// @brief Computes the distance between two iterators.
        /// @param lhs The later iterator
        /// @param rhs The earlier iterator
        /// @return Number of rows between the iterators; negative if lhs < rhs
        [[nodiscard]] friend difference_type operator-(
            const row_iterator& lhs, const row_iterator& rhs
        ) noexcept {
            return static_cast<difference_type>(lhs._row_idx - rhs._row_idx);
        }

        /// @brief Tests equality of two iterators.
        /// @param lhs Left iterator
        /// @param rhs Right iterator
        /// @return `true` if both iterators point to the same row index
        [[nodiscard]] friend bool operator==(
            const row_iterator& lhs, const row_iterator& rhs
        ) noexcept {
            return lhs._row_idx == rhs._row_idx;
        }

        /// @brief Three-way comparison of two iterators.
        /// @param lhs Left iterator
        /// @param rhs Right iterator
        /// @return Comparison result indicating iterator ordering
        [[nodiscard]] friend auto operator<=>(
            const row_iterator& lhs, const row_iterator& rhs
        ) noexcept {
            return lhs._row_idx <=> rhs._row_idx;
        }

    private:
        data_ptr_type _data_ptr{nullptr};
        size_type _row_size{0uz};
        size_type _row_idx{0uz};
    };

    /// @brief Mutable random access iterator over rows
    using iterator = row_iterator<false>;
    /// @brief Const random access iterator over rows
    using const_iterator = row_iterator<true>;
    /// @brief Reverse mutable iterator over rows
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// @brief Reverse const iterator over rows
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // --- constructors and assignment ---

    /// @brief Default constructor creates an empty `flat_matrix`.
    /// @post `empty() == true`, `n_rows() == 0`, `n_cols() == 0`, `data_size() == 0`
    flat_matrix() = default;

    /// @brief Copy constructor creates a deep copy of another `flat_matrix`.
    /// @param other The `flat_matrix` to copy
    /// @post `*this == other`
    flat_matrix(const flat_matrix&) = default;

    /// @brief Copy assignment creates a deep copy of another `flat_matrix`.
    /// @param other The source `flat_matrix`
    /// @return Reference to `*this`
    /// @post `*this == other`
    flat_matrix& operator=(const flat_matrix&) = default;

    /// @brief Move constructor transfers ownership of data from another `flat_matrix`.
    /// @param other The source `flat_matrix` (left in an empty state)
    /// @post `other.empty() == true`; all data is transferred to `*this`
    /// @warning Invalidates all iterators, pointers, and references to `other`'s elements.
    flat_matrix(flat_matrix&& other) noexcept
    : _n_rows(std::exchange(other._n_rows, 0uz)),
      _n_cols(std::exchange(other._n_cols, 0uz)),
      _data(std::move(other._data)) {}

    /// @brief Move assignment transfers ownership of data from another `flat_matrix`.
    /// @param other The source `flat_matrix`
    /// @return Reference to `*this`
    /// @post `other.empty() == true`; all data from `other` is transferred to `*this`
    /// @warning Invalidates all iterators, pointers, and references to this container's elements.
    /// @note This operator safely handles self-assignment.
    flat_matrix& operator=(flat_matrix&& other) noexcept {
        if (this != &other) {
            this->_n_rows = std::exchange(other._n_rows, 0uz);
            this->_n_cols = std::exchange(other._n_cols, 0uz);
            this->_data = std::move(other._data);
        }
        return *this;
    }

    /// @brief Destructor cleans up all managed memory.
    ~flat_matrix() = default;

    /// @brief Constructs a `flat_matrix` with specified dimensions.
    /// @param n_rows The number of rows
    /// @param n_cols The number of columns
    /// @param value The value to initialize all elements with (default constructed if omitted)
    /// @post `n_rows() == n_rows`, `n_cols() == n_cols`, and elements equal `value`
    /// @exception std::bad_alloc May throw if memory allocation fails
    flat_matrix(size_type n_rows, size_type n_cols, const value_type& value = value_type{})
    : _n_rows(n_rows), _n_cols(n_cols), _data(n_rows * n_cols, value) {}

    /// @brief Constructs a `flat_matrix` from an initializer list of rows.
    /// @param ilist Initializer list of initializer lists, each representing a row
    /// @post Dimensions are established based on the list geometry
    /// @exception std::invalid_argument If the rows in the list do not have identical lengths
    /// @exception std::bad_alloc May throw if memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references after construction
    flat_matrix(std::initializer_list<std::initializer_list<value_type>> ilist) {
        this->_n_rows = ilist.size();
        if (this->_n_rows == 0uz)
            return;

        this->_n_cols = ilist.begin()->size();
        this->_data.reserve(this->_n_rows * this->_n_cols);

        for (const auto& row : ilist) {
            if (row.size() != this->_n_cols) {
                throw std::invalid_argument(std::format(
                    "flat_matrix: row size mismatch in initializer_list (expected {}, got {})",
                    this->_n_cols,
                    row.size()
                ));
            }
            this->_data.insert(this->_data.end(), row.begin(), row.end());
        }
    }

    /// @brief Constructs a `flat_matrix` from a 2D range of ranges.
    ///
    /// The matrix establishes its column count from the size of the first extracted row.
    /// All subsequent rows must perfectly match this dimension. Provides a strong exception guarantee
    /// if the source is an unsized pure `input_range` and fails validation mid-extraction.
    ///
    /// @tparam R A range type whose elements are input ranges of `value_type`
    /// @param r The 2D range to initialize from
    /// @post Dimensions match the structure of `r`
    /// @exception std::invalid_argument If any extracted row size mismatches the first row's size
    /// @exception std::bad_alloc May throw if memory allocation fails
    template <std::ranges::input_range R>
    requires std::ranges::input_range<std::ranges::range_reference_t<R>>
         and std::convertible_to<
                 std::ranges::range_reference_t<std::ranges::range_reference_t<R>>,
                 value_type>
    explicit flat_matrix(R&& r) {
        if constexpr (std::ranges::sized_range<R>)
            this->_n_rows = std::ranges::size(r);

        bool first = true;
        for (auto&& subrange : r) {
            // can't consume an input range
            // size has to be determined by consuming the range and counting how many elements were inserted into _data
            const auto old_size = this->_data.size();

            if constexpr (std::ranges::contiguous_range<decltype(subrange)>) {
                auto* ptr = std::ranges::data(subrange);
                const auto dist = std::ranges::size(subrange);
                this->_data.insert(this->_data.end(), ptr, ptr + dist);
            }
            else {
                this->_data.insert(
                    this->_data.end(), std::ranges::begin(subrange), std::ranges::end(subrange)
                );
            }

            const auto row_size = this->_data.size() - old_size;
            if (first) {
                this->_n_cols = row_size;

                // prevent reallocation during loop for sized range
                if constexpr (std::ranges::sized_range<R>)
                    this->_data.reserve(this->_n_rows * this->_n_cols);

                first = false;
            }
            else if (row_size != this->_n_cols) {
                throw std::invalid_argument(std::format(
                    "flat_matrix: row size mismatch in range constructor (expected {}, got {})",
                    this->_n_cols,
                    row_size
                ));
            }
        }

        // calculate the number of rows for an unsized range
        if constexpr (not std::ranges::sized_range<R>)
            this->_n_rows = this->_data.size() / (this->_n_cols > 0uz ? this->_n_cols : 1uz);
    }

    // --- comparison ---

    /// @brief Tests equality of two `flat_matrix` instances.
    /// @param lhs Left operand
    /// @param rhs Right operand
    /// @return `true` if dimensions and all elements match
    friend bool operator==(const flat_matrix&, const flat_matrix&) = default;

    // --- size and capacity ---

    /// @brief Returns the number of rows in the matrix.
    /// @return The count of rows
    /// @note Required to idiomaticaly satisfy `std::ranges::sized_range`.
    [[nodiscard]] size_type size() const noexcept {
        return this->_n_rows;
    }

    /// @brief Returns the number of rows in the matrix.
    /// @return The count of rows
    [[nodiscard]] size_type n_rows() const noexcept {
        return this->_n_rows;
    }

    /// @brief Returns the number of columns in the matrix.
    /// @return The count of columns
    [[nodiscard]] size_type n_cols() const noexcept {
        return this->_n_cols;
    }

    /// @brief Checks if the container is entirely empty.
    /// @return `true` if `data_size() == 0`, `false` otherwise
    [[nodiscard]] bool empty() const noexcept {
        return this->_data.empty();
    }

    /// @brief Returns the current capacity for data elements.
    /// @return The number of total elements that can be stored in `_data` without reallocation
    [[nodiscard]] size_type data_capacity() const noexcept {
        return this->_data.capacity();
    }

    /// @brief Reserves space for at least n total elements without changing the dimensions.
    /// @param n The total number of matrix elements to reserve space for
    /// @post `data_capacity() >= n`
    /// @warning Invalidates all iterators and pointers to elements if reallocation occurs
    void reserve_data(size_type n) {
        this->_data.reserve(n);
    }

    /// @brief Reduces capacity of the internal array to match the current data size.
    /// @post `data_capacity() == data_size()`
    /// @warning Invalidates all iterators, pointers, and references to elements if reallocation occures
    void shrink_to_fit() {
        this->_data.shrink_to_fit();
    }

    /// @brief Resizes the mathematical dimensions of the matrix.
    ///
    /// If the new dimensions require structural changes (e.g. changing the number of columns),
    /// the mathematical grid is rebuilt and existing items are relocated to their new coordinate slots.
    ///
    /// @param new_rows The new number of rows
    /// @param new_cols The new number of columns
    /// @param value The value to initialize any newly exposed slots with
    /// @post `n_rows() == new_rows` and `n_cols() == new_cols`
    /// @warning Invalidates all iterators, pointers, and references.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are the new dimensions, due to remapping
    ///       elements in 2D space. If only the row count changes, it is $O(K)$ where $K$ is the number of
    ///       inserted or removed trailing elements.
    void resize(size_type new_rows, size_type new_cols, const value_type& value = value_type{}) {
        if (new_rows == this->_n_rows and new_cols == this->_n_cols)
            return;

        if (new_cols == this->_n_cols) {
            this->_data.resize(new_rows * new_cols, value);
            this->_n_rows = new_rows;
            return;
        }

        std::vector<value_type> new_data(new_rows * new_cols, value);
        const auto min_rows = std::min(this->_n_rows, new_rows);
        const auto min_cols = std::min(this->_n_cols, new_cols);

        for (auto r = 0uz; r < min_rows; ++r)
            for (auto c = 0uz; c < min_cols; ++c)
                new_data[r * new_cols + c] = std::move(this->_data[r * this->_n_cols + c]);

        this->_data = std::move(new_data);
        this->_n_rows = new_rows;
        this->_n_cols = new_cols;
    }

    /// @brief Removes all dimensions and elements, leaving the matrix empty.
    /// @post `n_rows() == 0`, `n_cols() == 0`, `data_size() == 0`
    /// @warning Invalidates all iterators, pointers, and references to elements
    void clear() {
        this->_data.clear();
        this->_n_rows = 0uz;
        this->_n_cols = 0uz;
    }

    // --- accessors ---

    /// @brief Computes the underlying flattened 1D index for a 2D coordinate.
    /// @param r The row index
    /// @param c The column index
    /// @return The 1D index mapping for `_data`
    /// @note Provides $O(1)$ constant time lookup calculation
    [[nodiscard]] constexpr size_type index(size_type r, size_type c) const noexcept {
        return r * this->_n_cols + c;
    }

    /// @brief Returns the row at the given index without bounds checking.
    /// @param r The index of the row to access
    /// @return A span representing the row at index r
    /// @pre `r < n_rows()`; otherwise Undefined Behavior
    /// @warning No bounds checking is performed for performance.
    [[nodiscard]] row_type operator[](size_type r) {
        return row_type(this->_data.data() + r * this->_n_cols, this->_n_cols);
    }

    /// @brief Returns a const row at the given index without bounds checking.
    /// @param r The index of the row to access
    /// @return A const span representing the row at index r
    /// @pre `r < n_rows()`; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] const_row_type operator[](size_type r) const {
        return const_row_type(this->_data.data() + r * this->_n_cols, this->_n_cols);
    }

    /// @brief Returns a reference to an element without bounds checking.
    /// @param r The row index
    /// @param c The column index
    /// @return Reference to the element at the given coordinates
    /// @pre `r < n_rows()` and `c < n_cols()`; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] reference operator[](size_type r, size_type c) {
        return this->_data[this->index(r, c)];
    }

    /// @brief Returns a const reference to an element without bounds checking.
    /// @param r The row index
    /// @param c The column index
    /// @return Const reference to the element at the given coordinates
    /// @pre `r < n_rows()` and `c < n_cols()`; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] const_reference operator[](size_type r, size_type c) const {
        return this->_data[this->index(r, c)];
    }

    /// @brief Returns the row at the given index with bounds checking.
    /// @param r The index of the row
    /// @return A span representing the row at index r
    /// @exception std::out_of_range If `r >= n_rows()`
    [[nodiscard]] row_type at(size_type r) {
        this->_check_row(r);
        return (*this)[r];
    }

    /// @brief Returns a const row at the given index with bounds checking.
    /// @param r The index of the row
    /// @return A const span representing the row at index r
    /// @exception std::out_of_range If `r >= n_rows()`
    [[nodiscard]] const_row_type at(size_type r) const {
        this->_check_row(r);
        return (*this)[r];
    }

    /// @brief Returns a reference to an element with bounds checking.
    /// @param r The row index
    /// @param c The column index
    /// @return Reference to the element
    /// @exception std::out_of_range If `r >= n_rows()` or `c >= n_cols()`
    [[nodiscard]] reference at(size_type r, size_type c) {
        this->_check_row(r);
        this->_check_col(c);
        return (*this)[r, c];
    }

    /// @brief Returns a const reference to an element with bounds checking.
    /// @param r The row index
    /// @param c The column index
    /// @return Const reference to the element
    /// @exception std::out_of_range If `r >= n_rows()` or `c >= n_cols()`
    [[nodiscard]] const_reference at(size_type r, size_type c) const {
        this->_check_row(r);
        this->_check_col(c);
        return (*this)[r, c];
    }

    /// @brief Returns the first row without bounds checking.
    /// @return A span representing the first row
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] row_type front() noexcept {
        return (*this)[0uz];
    }

    /// @brief Returns a const reference to the first row without bounds checking.
    /// @return A const span representing the first row
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] const_row_type front() const noexcept {
        return (*this)[0uz];
    }

    /// @brief Returns the last row without bounds checking.
    /// @return A span representing the last row
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] row_type back() noexcept {
        return (*this)[this->_n_rows - 1uz];
    }

    /// @brief Returns a const reference to the last row without bounds checking.
    /// @return A const span representing the last row
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] const_row_type back() const noexcept {
        return (*this)[this->_n_rows - 1uz];
    }

    /// @brief Explicitly named alias for `front()` yielding the first row.
    /// @return A span representing the first row
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    [[nodiscard]] row_type front_row() noexcept {
        return this->front();
    }

    /// @brief Explicitly named alias for `front()` yielding the first const row.
    /// @return A const span representing the first row
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    [[nodiscard]] const_row_type front_row() const noexcept {
        return this->front();
    }

    /// @brief Explicitly named alias for `back()` yielding the last row.
    /// @return A span representing the last row
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    [[nodiscard]] row_type back_row() noexcept {
        return this->back();
    }

    /// @brief Explicitly named alias for `back()` yielding the last const row.
    /// @return A const span representing the last row
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    [[nodiscard]] const_row_type back_row() const noexcept {
        return this->back();
    }

    /// @brief Returns an unchecked $O(1)$ random-access view over the first column.
    /// @return A strided view representing the first column
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] auto front_col() noexcept {
        return this->_col_impl(0uz);
    }

    /// @brief Returns an unchecked $O(1)$ random-access const view over the first column.
    /// @return A strided view representing the const first column
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] auto front_col() const noexcept {
        return this->_col_impl(0uz);
    }

    /// @brief Returns an unchecked $O(1)$ random-access view over the last column.
    /// @return A strided view representing the last column
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] auto back_col() noexcept {
        return this->_col_impl(this->_n_cols - 1uz);
    }

    /// @brief Returns an unchecked $O(1)$ random-access const view over the last column.
    /// @return A strided view representing the const last column
    /// @pre Matrix must not be empty; otherwise Undefined Behavior
    /// @warning No bounds checking is performed.
    [[nodiscard]] auto back_col() const noexcept {
        return this->_col_impl(this->_n_cols - 1uz);
    }

    /// @brief Semantically symmetric alias for `at(r)` returning a bounds-checked row.
    /// @param r The row index
    /// @return A span representing the row
    /// @exception std::out_of_range If `r >= n_rows()`
    [[nodiscard]] row_type row(size_type r) {
        return this->at(r);
    }

    /// @brief Semantically symmetric alias for `at(r)` returning a bounds-checked const row.
    /// @param r The row index
    /// @return A const span representing the row
    /// @exception std::out_of_range If `r >= n_rows()`
    [[nodiscard]] const_row_type row(size_type r) const {
        return this->at(r);
    }

    /// @brief Returns a bounds-checked $O(1)$ random-access view over a specific column.
    /// @param c The column index
    /// @return A strided view representing the column
    /// @exception std::out_of_range If `c >= n_cols()`
    [[nodiscard]] auto col(size_type c) {
        this->_check_col(c);
        return this->_col_impl(c);
    }

    /// @brief Returns a bounds-checked $O(1)$ random-access const view over a specific column.
    /// @param c The column index
    /// @return A strided view representing the const column
    /// @exception std::out_of_range If `c >= n_cols()`
    [[nodiscard]] auto col(size_type c) const {
        this->_check_col(c);
        return this->_col_impl(c);
    }

    /// @brief Returns a view of all rows for iteration.
    /// @return A random-access view of all row spans
    [[nodiscard]] auto rows() noexcept {
        return std::views::iota(size_type{0}, this->_n_rows)
             | std::views::transform([this](size_type i) -> row_type { return (*this)[i]; });
    }

    /// @brief Returns a const view of all rows for iteration.
    /// @return A random-access const view of all row const spans
    [[nodiscard]] auto rows() const noexcept {
        return std::views::iota(size_type{0}, this->_n_rows)
             | std::views::transform([this](size_type i) -> const_row_type { return (*this)[i]; });
    }

    /// @brief Returns a view of all columns for iteration.
    /// @return A random-access view of all column strided-views
    [[nodiscard]] auto cols() noexcept {
        return std::views::iota(size_type{0}, this->_n_cols)
             | std::views::transform([this](size_type c) { return this->_col_impl(c); });
    }

    /// @brief Returns a const view of all columns for iteration.
    /// @return A random-access view of all const column strided-views
    [[nodiscard]] auto cols() const noexcept {
        return std::views::iota(size_type{0}, this->_n_cols)
             | std::views::transform([this](size_type c) { return this->_col_impl(c); });
    }

    // --- accessors (data) ---

    /// @brief Returns the total number of elements structurally stored in the matrix.
    /// @return The result of `n_rows() * n_cols()`
    [[nodiscard]] size_type data_size() const noexcept {
        return this->_data.size();
    }

    /// @brief Returns a span over all element data in flattened 1D form.
    /// @return A span of all elements in the underlying `_data` array.
    [[nodiscard]] row_type data_view() noexcept {
        return row_type(this->_data);
    }

    /// @brief Returns a const span over all element data in flattened 1D form.
    /// @return A const span of all elements in the underlying `_data` array.
    [[nodiscard]] const_row_type data_view() const noexcept {
        return const_row_type(this->_data);
    }

    /// @brief Returns a reference to the underlying flat data container.
    /// @return A mutable reference to the underlying `_data` array.
    /// @warning Modifying this vector directly can fatally corrupt the matrix structure. Use for advanced operations only.
    [[nodiscard]] std::vector<value_type>& data_storage() noexcept {
        return this->_data;
    }

    /// @brief Returns a const reference to the underlying flat data container.
    /// @return A const reference to the underlying `_data` array.
    [[nodiscard]] const std::vector<value_type>& data_storage() const noexcept {
        return this->_data;
    }

    /// @brief Returns a raw pointer to the underlying flat data array.
    /// @return A raw pointer to the first element in the `_data` array.
    /// @warning No bounds checking is performed.
    [[nodiscard]] value_type* data_ptr() noexcept {
        return this->_data.data();
    }

    /// @brief Returns a const raw pointer to the underlying flat data array.
    /// @return A const raw pointer to the first element in the `_data` array.
    [[nodiscard]] const value_type* data_ptr() const noexcept {
        return this->_data.data();
    }

    // --- modifiers (rows) ---

    /// @brief Appends a range as a new row at the bottom of the matrix.
    /// @tparam R An input range of elements convertible to `value_type`
    /// @param r The range to append
    /// @post `n_rows()` increases by 1
    /// @exception std::invalid_argument If the row size does not match `n_cols()` (for non-empty matrices)
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references if reallocation occurs.
    /// @note **Time Complexity:** Amortized $O(C)$ where $C$ is the number of columns.
    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void push_row(R&& r) {
        this->insert_row(this->_n_rows, std::forward<R>(r));
    }

    /// @brief Appends an initializer list as a new row at the bottom of the matrix.
    /// @param ilist The list to append
    /// @post `n_rows()` increases by 1
    /// @exception std::invalid_argument If the list size does not match `n_cols()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references if reallocation occurs.
    /// @note **Time Complexity:** Amortized $O(C)$ where $C$ is the number of columns.
    void push_row(std::initializer_list<value_type> ilist) {
        this->insert_row(this->_n_rows, std::span<const value_type>{ilist});
    }

    /// @brief Appends a newly created row filled with a specific value.
    ///
    /// If the matrix is empty (has no columns), this operation has no effect,
    /// as the size of the new row will be determined as 0.
    ///
    /// @param value The value to fill the new row with
    /// @post `n_rows()` increases by 1
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references if reallocation occurs.
    /// @note If the matrix is empty, nothing will happen.
    /// @note **Time Complexity:** Amortized $O(C)$ where $C$ is the number of columns.
    void push_row(const value_type& value) {
        this->insert_row(this->_n_rows, value);
    }

    /// @brief Inserts a new row at the specified position from a range.
    /// @tparam R An input range of elements convertible to `value_type`
    /// @param pos The row position where the elements will be inserted
    /// @param r The range to insert
    /// @post `n_rows()` increases by 1; rows at and after `pos` are shifted down
    /// @exception std::out_of_range If `pos > n_rows()`
    /// @exception std::invalid_argument If the range size does not match `n_cols()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @note Provides strong exception guarantee if size validation fails for unsized ranges.
    /// @warning Invalidates all iterators, pointers, and references after the insertion point.
    /// @note **Time Complexity:** $O(E + C)$ where $E$ is the number of total elements from `pos` onward
    ///       and $C$ is the size of the inserted row.
    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void insert_row(size_type pos, R&& r) {
        if (pos > this->_n_rows) {
            throw std::out_of_range(std::format(
                "flat_matrix::insert_row: pos (which is {}) > this->rows() (which is {})",
                pos,
                this->_n_rows
            ));
        }

        const auto insert_pos = static_cast<std::ptrdiff_t>(pos * this->_n_cols);

        if constexpr (std::ranges::sized_range<R>) {
            const auto row_size = static_cast<size_type>(std::ranges::size(r));
            if (this->_n_rows > 0uz and row_size != this->_n_cols) {
                throw std::invalid_argument(std::format(
                    "flat_matrix::insert_row: row size mismatch (expected {}, got {})",
                    this->_n_cols,
                    row_size
                ));
            }

            if (this->_n_rows == 0uz and this->_n_cols == 0uz)
                this->_n_cols = row_size;

            if constexpr (std::ranges::contiguous_range<R>) {
                auto* ptr = std::ranges::data(r);
                this->_data.insert(this->_data.begin() + insert_pos, ptr, ptr + row_size);
            }
            else {
                this->_data.insert(
                    this->_data.begin() + insert_pos, std::ranges::begin(r), std::ranges::end(r)
                );
            }
        }
        else {
            // single-pass input range: insert, validate size, rollback if mismatched (strong exception guarantee)
            const auto old_size = this->_data.size();

            this->_data.insert(
                this->_data.begin() + insert_pos, std::ranges::begin(r), std::ranges::end(r)
            );

            const auto row_size = this->_data.size() - old_size;
            if (this->_n_rows > 0uz and row_size != this->_n_cols) {
                this->_data.erase(
                    this->_data.begin() + insert_pos, this->_data.begin() + insert_pos + row_size
                );
                throw std::invalid_argument(std::format(
                    "flat_matrix::insert_row: row size mismatch (expected {}, got {})",
                    this->_n_cols,
                    row_size
                ));
            }

            if (this->_n_rows == 0uz and this->_n_cols == 0uz)
                this->_n_cols = row_size;
        }

        ++this->_n_rows;
    }

    /// @brief Inserts a new row at the specified position from an initializer list.
    /// @param pos The row position where the elements will be inserted
    /// @param ilist The list to insert
    /// @post `n_rows()` increases by 1; rows at and after `pos` are shifted down
    /// @exception std::out_of_range If `pos > n_rows()`
    /// @exception std::invalid_argument If the list size does not match `n_cols()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references after the insertion point.
    /// @note **Time Complexity:** $O(E + C)$ where $E$ is the number of total elements from `pos` onward
    ///       and $C$ is the size of the inserted row.
    void insert_row(size_type pos, std::initializer_list<value_type> ilist) {
        this->insert_row(pos, std::span<const value_type>{ilist});
    }

    /// @brief Inserts a newly created row filled with a specific value at the specified position.
    ///
    /// If the matrix is empty (has no columns), this operation has no effect,
    /// as the size of the new row will be determined as 0.
    ///
    /// @param pos The row position where the elements will be inserted
    /// @param value The value to fill the new row with
    /// @post `n_rows()` increases by 1; rows at and after `pos` are shifted down
    /// @exception std::out_of_range If `pos > n_rows()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning Invalidates all iterators, pointers, and references after the insertion point.
    /// @note **Time Complexity:** $O(E + C)$ where $E$ is the number of total elements from `pos` onward
    ///       and $C$ is the size of the inserted row.
    void insert_row(size_type pos, const value_type& value) {
        if (pos > this->_n_rows) {
            throw std::out_of_range(std::format(
                "flat_matrix::insert_row: pos (which is {}) > this->rows() (which is {})",
                pos,
                this->_n_rows
            ));
        }

        const auto insert_pos = static_cast<std::ptrdiff_t>(pos * this->_n_cols);
        this->_data.insert(this->_data.begin() + insert_pos, this->_n_cols, value);
        ++this->_n_rows;
    }

    /// @brief Removes the last row from the matrix.
    /// @post If not empty, `n_rows()` decreases by 1
    /// @note Safe to call on an empty matrix (no-op).
    /// @warning Invalidates all iterators, pointers, and references to elements in the last row.
    /// @note **Time Complexity:** $O(C)$ to truncate the underlying `_data` vector.
    void pop_row() {
        if (this->empty())
            return;

        this->_data.resize(this->_data.size() - this->_n_cols);
        --this->_n_rows;

        if (this->_n_rows == 0uz)
            this->_n_cols = 0uz;
    }

    /// @brief Erases the row at the specified position.
    /// @param pos The position of the row to erase
    /// @post The row is removed; subsequent rows are shifted up; `n_rows()` decreases by 1
    /// @exception std::out_of_range If `pos >= n_rows()`
    /// @warning Invalidates all iterators, pointers, and references at or after the erased position.
    /// @note **Time Complexity:** $O(E + C)$ where $E$ is the number of elements after the erased row
    ///       and $C$ is the number of columns (the size of the erased row).
    void erase_row(size_type pos) {
        this->_check_row(pos);
        if (this->_n_rows == 1uz) {
            this->clear();
            return;
        }

        const auto start_it =
            this->_data.begin() + static_cast<std::ptrdiff_t>(pos * this->_n_cols);
        this->_data.erase(start_it, start_it + static_cast<std::ptrdiff_t>(this->_n_cols));
        --this->_n_rows;
    }

    // --- modifiers (columns) ---

    /// @brief Appends a range as a new column at the right edge of the matrix.
    /// @tparam R An input range of elements convertible to `value_type`
    /// @param r The range to append
    /// @post `n_cols()` increases by 1
    /// @exception std::invalid_argument If the column size does not match `n_rows()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning This operation forces a full reallocation and architectural shift of the mathematical grid.
    ///          All iterators, pointers, and references are invalidated.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are dimensions of the matrix.
    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void push_col(R&& r) {
        this->insert_col(this->_n_cols, std::forward<R>(r));
    }

    /// @brief Appends an initializer list as a new column at the right edge of the matrix.
    /// @param ilist The list to append
    /// @post `n_cols()` increases by 1
    /// @exception std::invalid_argument If the list size does not match `n_rows()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning This operation forces a full reallocation and architectural shift of the mathematical grid.
    ///          All iterators, pointers, and references are invalidated.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are dimensions of the matrix.
    void push_col(std::initializer_list<value_type> ilist) {
        this->insert_col(this->_n_cols, std::span<const value_type>{ilist});
    }

    /// @brief Appends a newly created column filled with a specific value at the right edge.
    ///
    /// If the matrix is empty (has no rows), this operation has no effect,
    /// as the size of the new column will be determined as 0.
    ///
    /// @param value The value to fill the new column with
    /// @post `n_cols()` increases by 1
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning This operation forces a full reallocation and architectural shift of the mathematical grid.
    ///          All iterators, pointers, and references are invalidated.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are dimensions of the matrix.
    void push_col(const value_type& value) {
        this->insert_col(this->_n_cols, value);
    }

    /// @brief Inserts a new column at the specified position from a range.
    /// @tparam R An input range of elements convertible to `value_type`
    /// @param pos The column position where elements will be inserted
    /// @param r The range to insert
    /// @post `n_cols()` increases by 1
    /// @exception std::out_of_range If `pos > n_cols()`
    /// @exception std::invalid_argument If the range size does not match `n_rows()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning This operation forces a full reallocation and architectural shift of the mathematical grid.
    ///          All iterators, pointers, and references are invalidated.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are dimensions of the matrix.
    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void insert_col(size_type pos, R&& r) {
        if (pos > this->_n_cols) {
            throw std::out_of_range(std::format(
                "flat_matrix::insert_col: pos (which is {}) > this->cols() (which is {})",
                pos,
                this->_n_cols
            ));
        }

        if constexpr (std::ranges::sized_range<R>) {
            const auto col_size = static_cast<size_type>(std::ranges::size(r));
            if (this->_n_cols > 0uz and col_size != this->_n_rows) {
                throw std::invalid_argument(std::format(
                    "flat_matrix::insert_col: col size mismatch (expected {}, got {})",
                    this->_n_rows,
                    col_size
                ));
            }

            if (this->_n_rows == 0uz and this->_n_cols == 0uz)
                this->_n_rows = col_size;

            // pre-allocate new vector to guarantee O(RxC) structural shift, avoiding cubic complexity with multiple inserts
            std::vector<value_type> new_data;
            new_data.reserve(this->_n_rows * (this->_n_cols + 1uz));

            auto r_it = std::ranges::begin(r);
            const auto n_rows_bound = static_cast<std::ptrdiff_t>(this->_n_rows);
            const auto row_size = static_cast<std::ptrdiff_t>(this->_n_cols);
            const auto c_pos = static_cast<std::ptrdiff_t>(pos);
            for (auto r_pos = 0z; r_pos < n_rows_bound; ++r_pos) {
                auto row_begin = this->_data.begin() + r_pos * row_size;

                // move old row elements up to insertion point
                new_data.insert(
                    new_data.end(),
                    std::make_move_iterator(row_begin),
                    std::make_move_iterator(row_begin + c_pos)
                );
                // insert new column element
                new_data.push_back(*r_it++);
                // move the remainder of old row
                new_data.insert(
                    new_data.end(),
                    std::make_move_iterator(row_begin + c_pos),
                    std::make_move_iterator(row_begin + row_size)
                );
            }

            this->_data = std::move(new_data);
            ++this->_n_cols;
        }
        else {
            // create a temporary sized range and recursively call the method to leverage the sized range logic,
            // ensuring strong exception guarantee for unsized input
            this->insert_col(pos, std::ranges::to<std::vector<value_type>>(std::forward<R>(r)));
        }
    }

    /// @brief Inserts a new column at the specified position from an initializer list.
    /// @param pos The column position where elements will be inserted
    /// @param ilist The list to insert
    /// @post `n_cols()` increases by 1
    /// @exception std::out_of_range If `pos > n_cols()`
    /// @exception std::invalid_argument If the list size does not match `n_rows()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning This operation forces a full reallocation and architectural shift of the mathematical grid.
    ///          All iterators, pointers, and references are invalidated.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are dimensions of the matrix.
    void insert_col(size_type pos, std::initializer_list<value_type> ilist) {
        this->insert_col(pos, std::span<const value_type>{ilist});
    }

    /// @brief Inserts a newly created column filled with a specific value at the specified position.
    ///
    /// If the matrix is empty (has no rows), this operation has no effect,
    /// as the size of the new column will be determined as 0.
    ///
    /// @param pos The column position where elements will be inserted
    /// @param value The value to fill the new column with
    /// @post `n_cols()` increases by 1
    /// @exception std::out_of_range If `pos > n_cols()`
    /// @exception std::bad_alloc If memory allocation fails
    /// @warning This operation forces a full reallocation and architectural shift of the mathematical grid.
    ///          All iterators, pointers, and references are invalidated.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are dimensions of the matrix.
    void insert_col(size_type pos, const value_type& value) {
        if (pos > this->_n_cols) {
            throw std::out_of_range(std::format(
                "flat_matrix::insert_col: pos (which is {}) > this->cols() (which is {})",
                pos,
                this->_n_cols
            ));
        }

        std::vector<value_type> new_data;
        new_data.reserve(this->_n_rows * (this->_n_cols + 1uz));

        const auto n_rows_bound = static_cast<std::ptrdiff_t>(this->_n_rows);
        const auto row_size = static_cast<std::ptrdiff_t>(this->_n_cols);
        const auto c_pos = static_cast<std::ptrdiff_t>(pos);
        for (auto r_pos = 0z; r_pos < n_rows_bound; ++r_pos) {
            auto row_begin = this->_data.begin() + r_pos * row_size;

            new_data.insert(
                new_data.end(),
                std::make_move_iterator(row_begin),
                std::make_move_iterator(row_begin + c_pos)
            );
            new_data.push_back(value); // Insert the fill value
            new_data.insert(
                new_data.end(),
                std::make_move_iterator(row_begin + c_pos),
                std::make_move_iterator(row_begin + row_size)
            );
        }

        this->_data = std::move(new_data);
        ++this->_n_cols;
    }

    /// @brief Removes the last column from the matrix.
    /// @post If not empty, `n_cols()` decreases by 1
    /// @note Safe to call on an empty matrix (no-op).
    /// @warning This operation forces a reallocation and structural shift. All iterators, pointers, and references are invalidated.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are dimensions of the matrix.
    void pop_col() {
        if (this->empty() || this->_n_cols == 0uz)
            return;

        this->erase_col(this->_n_cols - 1uz);
    }

    /// @brief Erases the column at the specified position.
    /// @param pos The position of the column to erase
    /// @post The column is removed; subsequent columns are mathematically shifted left; `n_cols()` decreases by 1
    /// @exception std::out_of_range If `pos >= n_cols()`
    /// @warning This operation forces a reallocation and structural shift. All iterators, pointers, and references are invalidated.
    /// @note **Time Complexity:** $O(R \times C)$ where $R$ and $C$ are dimensions of the matrix.
    void erase_col(size_type pos) {
        this->_check_col(pos);

        if (this->_n_cols == 1uz) {
            this->clear();
            return;
        }

        std::vector<value_type> new_data;
        new_data.reserve(this->_n_rows * (this->_n_cols - 1uz));

        const auto n_rows_bound = static_cast<std::ptrdiff_t>(this->_n_rows);
        const auto row_size = static_cast<std::ptrdiff_t>(this->_n_cols);
        const auto c_pos = static_cast<std::ptrdiff_t>(pos);
        for (auto r_pos = 0z; r_pos < n_rows_bound; ++r_pos) {
            auto row_begin = this->_data.begin() + r_pos * row_size;

            new_data.insert(
                new_data.end(),
                std::make_move_iterator(row_begin),
                std::make_move_iterator(row_begin + c_pos)
            );
            new_data.insert(
                new_data.end(),
                std::make_move_iterator(row_begin + c_pos + 1z),
                std::make_move_iterator(row_begin + row_size)
            );
        }

        this->_data = std::move(new_data);
        --this->_n_cols;
    }

    // --- iterators ---

    /// @brief Returns a mutable iterator to the first row.
    /// @return Iterator to the first row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] iterator begin() noexcept {
        return iterator(this->_data.data(), this->_n_cols, 0uz);
    }

    /// @brief Returns a mutable iterator past the last row (end sentinel).
    /// @return Iterator one position past the last row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] iterator end() noexcept {
        return iterator(this->_data.data(), this->_n_cols, this->_n_rows);
    }

    /// @brief Returns a const iterator to the first row.
    /// @return Const iterator to the first row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_iterator begin() const noexcept {
        return const_iterator(this->_data.data(), this->_n_cols, 0uz);
    }

    /// @brief Returns a const iterator past the last row (end sentinel).
    /// @return Const iterator one position past the last row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_iterator end() const noexcept {
        return const_iterator(this->_data.data(), this->_n_cols, this->_n_rows);
    }

    /// @brief Returns a const iterator to the first row (explicit const form).
    /// @return Const iterator to the first row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return this->begin();
    }

    /// @brief Returns a const iterator past the last row (explicit const form).
    /// @return Const iterator one past the last row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_iterator cend() const noexcept {
        return this->end();
    }

    /// @brief Returns a reverse iterator to the last row.
    /// @return Reverse iterator starting at the last row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] reverse_iterator rbegin() noexcept {
        return reverse_iterator(this->end());
    }

    /// @brief Returns a reverse iterator before the first row (end sentinel).
    /// @return Reverse iterator one position before the first row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] reverse_iterator rend() noexcept {
        return reverse_iterator(this->begin());
    }

    /// @brief Returns a const reverse iterator to the last row.
    /// @return Const reverse iterator starting at the last row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(this->end());
    }

    /// @brief Returns a const reverse iterator before the first row (end sentinel).
    /// @return Const reverse iterator one position before the first row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(this->begin());
    }

    /// @brief Returns a const reverse iterator to the last row (explicit const form).
    /// @return Const reverse iterator starting at the last row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return this->rbegin();
    }

    /// @brief Returns a const reverse iterator before the first row (explicit const form).
    /// @return Const reverse iterator one position before the first row
    /// @note Iterator invalidated by structural modifications
    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return this->rend();
    }

    // --- transformations ---

    /// @brief Transposes the matrix mathematically (rows become columns, columns become rows).
    /// @return A new `flat_matrix` instance containing the transposed data
    /// @note **Time Complexity:** $O(R \times C)$ to generate and fill the new matrix.
    [[nodiscard]] flat_matrix transpose() const {
        flat_matrix result(this->_n_cols, this->_n_rows);
        for (size_type r = 0uz; r < this->_n_rows; ++r)
            for (size_type c = 0uz; c < this->_n_cols; ++c)
                result[c, r] = (*this)[r, c];
        return result;
    }

private:
    /// @brief Validates that the row index is within mathematical bounds.
    /// @param r The row index to check
    /// @exception std::out_of_range If `r >= n_rows()`
    /// @note Used internally by checked accessors
    void _check_row(size_type r) const {
        if (r >= this->_n_rows) {
            throw std::out_of_range(std::format(
                "flat_matrix::_check_row: r (which is {}) >= this->n_rows() (which is {})",
                r,
                this->_n_rows
            ));
        }
    }

    /// @brief Validates that the column index is within mathematical bounds.
    /// @param c The column index to check
    /// @exception std::out_of_range If `c >= n_cols()`
    /// @note Used internally by checked accessors
    void _check_col(size_type c) const {
        if (c >= this->_n_cols) {
            throw std::out_of_range(std::format(
                "flat_matrix::_check_col: c (which is {}) >= this->n_cols() (which is {})",
                c,
                this->_n_cols
            ));
        }
    }

    /// @brief Internal non-throwing helper generating a mutable strided view over a column.
    /// @param c The column index (assumed valid)
    /// @return A zero-overhead `std::views::stride` representing the column elements
    [[nodiscard]] auto _col_impl(size_type c) noexcept {
        return std::views::drop(this->_data, static_cast<std::ptrdiff_t>(c))
             | std::views::stride(this->_n_cols);
    }

    /// @brief Internal non-throwing helper generating a const strided view over a column.
    /// @param c The column index (assumed valid)
    /// @return A zero-overhead `std::views::stride` representing the const column elements
    [[nodiscard]] auto _col_impl(size_type c) const noexcept {
        return std::views::drop(this->_data, static_cast<std::ptrdiff_t>(c))
             | std::views::stride(this->_n_cols);
    }

    size_type _n_rows{0uz};
    size_type _n_cols{0uz};
    std::vector<value_type> _data;
};

} // namespace gl
