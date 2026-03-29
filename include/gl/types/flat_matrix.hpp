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
#include <utility>
#include <vector>

namespace gl {

template <std::semiregular T>
class flat_matrix {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using row_type = std::span<value_type>;
    using const_row_type = std::span<const value_type>;

    // --- iterators ---

    template <bool Const>
    class row_iterator {
        using data_ptr_type = std::conditional_t<Const, const T*, T*>;

    public:
        using iterator_concept = std::random_access_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::conditional_t<Const, const_row_type, row_type>;
        using difference_type = std::ptrdiff_t;
        using pointer = void;
        using reference = value_type;

        row_iterator() = default;

        row_iterator(data_ptr_type data_ptr, size_type row_size, size_type row_idx) noexcept
        : _data_ptr(data_ptr), _row_size(row_size), _row_idx(row_idx) {}

        operator row_iterator<true>() const noexcept
        requires(not Const)
        {
            return row_iterator<true>(this->_data_ptr, this->_row_size, this->_row_idx);
        }

        [[nodiscard]] reference operator*() const noexcept {
            return reference(this->_data_ptr + this->_row_idx * this->_row_size, this->_row_size);
        }

        [[nodiscard]] reference operator[](difference_type n) const noexcept {
            return *(*this + n);
        }

        row_iterator& operator++() noexcept {
            ++this->_row_idx;
            return *this;
        }

        row_iterator operator++(int) noexcept {
            auto tmp = *this;
            ++this->_row_idx;
            return tmp;
        }

        row_iterator& operator--() noexcept {
            --this->_row_idx;
            return *this;
        }

        row_iterator operator--(int) noexcept {
            auto tmp = *this;
            --this->_row_idx;
            return tmp;
        }

        row_iterator& operator+=(difference_type n) noexcept {
            this->_row_idx += n;
            return *this;
        }

        row_iterator& operator-=(difference_type n) noexcept {
            this->_row_idx -= n;
            return *this;
        }

        [[nodiscard]] friend row_iterator operator+(row_iterator it, difference_type n) noexcept {
            return it += n;
        }

        [[nodiscard]] friend row_iterator operator+(difference_type n, row_iterator it) noexcept {
            return it += n;
        }

        [[nodiscard]] friend row_iterator operator-(row_iterator it, difference_type n) noexcept {
            return it -= n;
        }

        [[nodiscard]] friend difference_type operator-(
            const row_iterator& lhs, const row_iterator& rhs
        ) noexcept {
            return lhs._row_idx - rhs._row_idx;
        }

        [[nodiscard]] friend bool operator==(
            const row_iterator& lhs, const row_iterator& rhs
        ) noexcept {
            return lhs._row_idx == rhs._row_idx;
        }

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

    using iterator = row_iterator<false>;
    using const_iterator = row_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // --- constructors and assignment ---

    flat_matrix() = default;

    flat_matrix(const flat_matrix&) = default;
    flat_matrix& operator=(const flat_matrix&) = default;

    flat_matrix(flat_matrix&& other) noexcept
    : _n_rows(std::exchange(other._n_rows, 0uz)),
      _n_cols(std::exchange(other._n_cols, 0uz)),
      _data(std::move(other._data)) {}

    flat_matrix& operator=(flat_matrix&& other) noexcept {
        if (this != &other) {
            this->_n_rows = std::exchange(other._n_rows, 0uz);
            this->_n_cols = std::exchange(other._n_cols, 0uz);
            this->_data = std::move(other._data);
        }
        return *this;
    }

    ~flat_matrix() = default;

    flat_matrix(size_type n_rows, size_type n_cols, const value_type& initial_value = value_type{})
    : _n_rows(n_rows), _n_cols(n_cols), _data(n_rows * n_cols, initial_value) {}

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

                if constexpr (std::ranges::sized_range<
                                  R>) // prevent reallocation during loop for sized range
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

        if constexpr (not std::ranges::sized_range<
                          R>) // calculate the number of rows for an unsized range
            this->_n_rows = this->_data.size() / (this->_n_cols > 0uz ? this->_n_cols : 1uz);
    }

    // --- comparison ---

    friend bool operator==(const flat_matrix&, const flat_matrix&) = default;

    // --- size and capacity ---

    [[nodiscard]] size_type size() const noexcept {
        return this->_n_rows;
    }

    [[nodiscard]] size_type n_rows() const noexcept {
        return this->_n_rows;
    }

    [[nodiscard]] size_type n_cols() const noexcept {
        return this->_n_cols;
    }

    [[nodiscard]] bool empty() const noexcept {
        return this->_data.empty();
    }

    [[nodiscard]] size_type data_capacity() const noexcept {
        return this->_data.capacity();
    }

    void reserve_data(size_type n) {
        this->_data.reserve(n);
    }

    void shrink_to_fit() {
        this->_data.shrink_to_fit();
    }

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

    void clear() {
        this->_data.clear();
        this->_n_rows = 0uz;
        this->_n_cols = 0uz;
    }

    // --- accessors ---

    [[nodiscard]] constexpr size_type index(size_type r, size_type c) const noexcept {
        return r * this->_n_cols + c;
    }

    [[nodiscard]] row_type operator[](size_type r) {
        return row_type(this->_data.data() + r * this->_n_cols, this->_n_cols);
    }

    [[nodiscard]] const_row_type operator[](size_type r) const {
        return const_row_type(this->_data.data() + r * this->_n_cols, this->_n_cols);
    }

    [[nodiscard]] reference operator[](size_type r, size_type c) {
        return this->_data[this->index(r, c)];
    }

    [[nodiscard]] const_reference operator[](size_type r, size_type c) const {
        return this->_data[this->index(r, c)];
    }

    [[nodiscard]] row_type at(size_type r) {
        this->_check_row(r);
        return (*this)[r];
    }

    [[nodiscard]] const_row_type at(size_type r) const {
        this->_check_row(r);
        return (*this)[r];
    }

    [[nodiscard]] reference at(size_type r, size_type c) {
        this->_check_row(r);
        this->_check_col(c);
        return (*this)[r, c];
    }

    [[nodiscard]] const_reference at(size_type r, size_type c) const {
        this->_check_row(r);
        this->_check_col(c);
        return (*this)[r, c];
    }

    [[nodiscard]] row_type front() noexcept {
        return (*this)[0uz];
    }

    [[nodiscard]] const_row_type front() const noexcept {
        return (*this)[0uz];
    }

    [[nodiscard]] row_type back() noexcept {
        return (*this)[this->_n_rows - 1uz];
    }

    [[nodiscard]] const_row_type back() const noexcept {
        return (*this)[this->_n_rows - 1uz];
    }

    [[nodiscard]] reference front(size_type r) noexcept {
        return (*this)[r, 0uz];
    }

    [[nodiscard]] const_reference front(size_type r) const noexcept {
        return (*this)[r, 0uz];
    }

    [[nodiscard]] reference back(size_type r) noexcept {
        return (*this)[r, this->_n_cols - 1uz];
    }

    [[nodiscard]] const_reference back(size_type r) const noexcept {
        return (*this)[r, this->_n_cols - 1uz];
    }

    [[nodiscard]] auto rows() noexcept {
        return std::views::iota(size_type{0}, this->_n_rows)
             | std::views::transform([this](size_type i) -> row_type { return (*this)[i]; });
    }

    [[nodiscard]] auto rows() const noexcept {
        return std::views::iota(size_type{0}, this->_n_rows)
             | std::views::transform([this](size_type i) -> const_row_type { return (*this)[i]; });
    }

    [[nodiscard]] size_type data_size() const noexcept {
        return this->_data.size();
    }

    [[nodiscard]] row_type data_view() noexcept {
        return row_type(this->_data);
    }

    [[nodiscard]] const_row_type data_view() const noexcept {
        return const_row_type(this->_data);
    }

    [[nodiscard]] std::vector<value_type>& data_storage() noexcept {
        return this->_data;
    }

    [[nodiscard]] const std::vector<value_type>& data_storage() const noexcept {
        return this->_data;
    }

    [[nodiscard]] value_type* data_ptr() noexcept {
        return this->_data.data();
    }

    [[nodiscard]] const value_type* data_ptr() const noexcept {
        return this->_data.data();
    }

    // --- iterators ---

    [[nodiscard]] iterator begin() noexcept {
        return iterator(this->_data.data(), this->_n_cols, 0uz);
    }

    [[nodiscard]] iterator end() noexcept {
        return iterator(this->_data.data(), this->_n_cols, this->_n_rows);
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return const_iterator(this->_data.data(), this->_n_cols, 0uz);
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return const_iterator(this->_data.data(), this->_n_cols, this->_n_rows);
    }

    [[nodiscard]] const_iterator cbegin() const noexcept {
        return this->begin();
    }

    [[nodiscard]] const_iterator cend() const noexcept {
        return this->end();
    }

    [[nodiscard]] reverse_iterator rbegin() noexcept {
        return reverse_iterator(this->end());
    }

    [[nodiscard]] reverse_iterator rend() noexcept {
        return reverse_iterator(this->begin());
    }

    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(this->end());
    }

    [[nodiscard]] const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(this->begin());
    }

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return this->rbegin();
    }

    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return this->rend();
    }

    // --- modifiers (rows) ---

    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void push_row(R&& r) {
        this->insert_row(this->_n_rows, std::forward<R>(r));
    }

    void push_row(std::initializer_list<value_type> ilist) {
        this->insert_row(this->_n_rows, std::span<const value_type>{ilist});
    }

    void push_row(const value_type& value) {
        this->insert_row(this->_n_rows, value);
    }

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

        const auto insert_idx = pos * this->_n_cols;

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

            this->_data.reserve(this->_data.size() + row_size);

            if constexpr (std::ranges::contiguous_range<R>) {
                auto* ptr = std::ranges::data(r);
                this->_data.insert(this->_data.begin() + insert_idx, ptr, ptr + row_size);
            }
            else {
                this->_data.insert(
                    this->_data.begin() + insert_idx, std::ranges::begin(r), std::ranges::end(r)
                );
            }
        }
        else {
            // single-pass input range: insert, validate size, rollback if mismatched (strong exception guarantee)
            const auto old_size = this->_data.size();

            this->_data.insert(
                this->_data.begin() + insert_idx, std::ranges::begin(r), std::ranges::end(r)
            );

            const auto row_size = this->_data.size() - old_size;
            if (this->_n_rows > 0uz and row_size != this->_n_cols) {
                this->_data.erase(
                    this->_data.begin() + insert_idx, this->_data.begin() + insert_idx + row_size
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

    void insert_row(size_type pos, std::initializer_list<value_type> ilist) {
        this->insert_row(pos, std::span<const value_type>{ilist});
    }

    void insert_row(size_type pos, const value_type& value) {
        if (pos > this->_n_rows) {
            throw std::out_of_range(std::format(
                "flat_matrix::insert_row: pos (which is {}) > this->rows() (which is {})",
                pos,
                this->_n_rows
            ));
        }

        this->_data.insert(this->_data.begin() + (pos * this->_n_cols), this->_n_cols, value);
        ++this->_n_rows;
    }

    void pop_row() {
        if (this->empty())
            return;

        this->_data.resize(this->_data.size() - this->_n_cols);
        --this->_n_rows;

        if (this->_n_rows == 0uz)
            this->_n_cols = 0uz;
    }

    void erase_row(size_type pos) {
        this->_check_row(pos);
        if (this->_n_rows == 1uz) {
            this->clear();
            return;
        }

        const auto start_it = this->_data.begin() + (pos * this->_n_cols);
        this->_data.erase(start_it, start_it + this->_n_cols);
        --this->_n_rows;
    }

    // --- modifiers (columns) ---

    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void push_col(R&& r) {
        this->insert_col(this->_n_cols, std::forward<R>(r));
    }

    void push_col(std::initializer_list<value_type> ilist) {
        this->insert_col(this->_n_cols, std::span<const value_type>{ilist});
    }

    void push_col(const value_type& value) {
        this->insert_col(this->_n_cols, value);
    }

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

            // pre-allocate new vector to guarantee O(V^2) structural shift, avoiding O(V^3) with multiple inserts
            std::vector<value_type> new_data;
            new_data.reserve(this->_n_rows * (this->_n_cols + 1uz));

            auto r_it = std::ranges::begin(r);
            for (size_type r_idx = 0uz; r_idx < this->_n_rows; ++r_idx) {
                auto row_begin = this->_data.begin() + r_idx * this->_n_cols;

                // move old row elements up to insertion point
                new_data.insert(
                    new_data.end(),
                    std::make_move_iterator(row_begin),
                    std::make_move_iterator(row_begin + pos)
                );
                // insert new column element
                new_data.push_back(*r_it++);
                // move the remainder of old row
                new_data.insert(
                    new_data.end(),
                    std::make_move_iterator(row_begin + pos),
                    std::make_move_iterator(row_begin + this->_n_cols)
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

    void insert_col(size_type pos, std::initializer_list<value_type> ilist) {
        this->insert_col(pos, std::span<const value_type>{ilist});
    }

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

        for (size_type r_idx = 0uz; r_idx < this->_n_rows; ++r_idx) {
            auto row_begin = this->_data.begin() + r_idx * this->_n_cols;

            new_data.insert(
                new_data.end(),
                std::make_move_iterator(row_begin),
                std::make_move_iterator(row_begin + pos)
            );
            new_data.push_back(value); // Insert the fill value
            new_data.insert(
                new_data.end(),
                std::make_move_iterator(row_begin + pos),
                std::make_move_iterator(row_begin + this->_n_cols)
            );
        }

        this->_data = std::move(new_data);
        ++this->_n_cols;
    }

    void pop_col() {
        if (this->empty() || this->_n_cols == 0uz)
            return;

        this->erase_col(this->_n_cols - 1uz);
    }

    void erase_col(size_type pos) {
        this->_check_col(pos);

        if (this->_n_cols == 1uz) {
            this->clear();
            return;
        }

        std::vector<value_type> new_data;
        new_data.reserve(this->_n_rows * (this->_n_cols - 1uz));

        for (size_type r_idx = 0uz; r_idx < this->_n_rows; ++r_idx) {
            auto row_begin = this->_data.begin() + r_idx * this->_n_cols;

            new_data.insert(
                new_data.end(),
                std::make_move_iterator(row_begin),
                std::make_move_iterator(row_begin + pos)
            );
            new_data.insert(
                new_data.end(),
                std::make_move_iterator(row_begin + pos + 1uz),
                std::make_move_iterator(row_begin + this->_n_cols)
            );
        }

        this->_data = std::move(new_data);
        --this->_n_cols;
    }

private:
    void _check_row(size_type r) const {
        if (r >= this->_n_rows) {
            throw std::out_of_range(std::format(
                "flat_matrix::_check_row: r (which is {}) >= this->n_rows() (which is {})",
                r,
                this->_n_rows
            ));
        }
    }

    void _check_col(size_type c) const {
        if (c >= this->_n_cols) {
            throw std::out_of_range(std::format(
                "flat_matrix::_check_col: c (which is {}) >= this->n_cols() (which is {})",
                c,
                this->_n_cols
            ));
        }
    }

    size_type _n_rows{0uz};
    size_type _n_cols{0uz};
    std::vector<value_type> _data;
};

} // namespace gl
