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

template <std::semiregular T>
class segment_vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using segment_type = std::span<value_type>;
    using const_segment_type = std::span<const value_type>;

    // --- iterators ---

    template <bool Const>
    class segment_iterator {
        using data_ptr_type = std::conditional_t<Const, const T*, T*>;
        using offset_ptr_type = const size_type*;

    public:
        using iterator_concept = std::random_access_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::conditional_t<Const, const_segment_type, segment_type>;
        using difference_type = std::ptrdiff_t;
        using pointer = void;
        using reference = value_type;

        segment_iterator() = default;

        segment_iterator(data_ptr_type data_ptr, offset_ptr_type offset_ptr) noexcept
        : _data_ptr(data_ptr), _offset_ptr(offset_ptr) {}

        operator segment_iterator<true>() const noexcept
        requires(not Const)
        {
            return segment_iterator<true>(this->_data_ptr, this->_offset_ptr);
        }

        [[nodiscard]] reference operator*() const noexcept {
            const auto beg = *this->_offset_ptr;
            const auto end = *(this->_offset_ptr + 1uz);
            return reference(this->_data_ptr + beg, end - beg);
        }

        [[nodiscard]] reference operator[](difference_type n) const noexcept {
            return *(*this + n);
        }

        segment_iterator& operator++() noexcept {
            ++this->_offset_ptr;
            return *this;
        }

        segment_iterator operator++(int) noexcept {
            auto tmp = *this;
            ++this->_offset_ptr;
            return tmp;
        }

        segment_iterator& operator--() noexcept {
            --this->_offset_ptr;
            return *this;
        }

        segment_iterator operator--(int) noexcept {
            auto tmp = *this;
            --this->_offset_ptr;
            return tmp;
        }

        segment_iterator& operator+=(difference_type n) noexcept {
            this->_offset_ptr += n;
            return *this;
        }

        segment_iterator& operator-=(difference_type n) noexcept {
            this->_offset_ptr -= n;
            return *this;
        }

        [[nodiscard]] friend segment_iterator operator+(
            segment_iterator it, difference_type n
        ) noexcept {
            return it += n;
        }

        [[nodiscard]] friend segment_iterator operator+(
            difference_type n, segment_iterator it
        ) noexcept {
            return it += n;
        }

        [[nodiscard]] friend segment_iterator operator-(
            segment_iterator it, difference_type n
        ) noexcept {
            return it -= n;
        }

        [[nodiscard]] friend difference_type operator-(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr - rhs._offset_ptr;
        }

        [[nodiscard]] friend bool operator==(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr == rhs._offset_ptr;
        }

        [[nodiscard]] friend auto operator<=>(
            const segment_iterator& lhs, const segment_iterator& rhs
        ) noexcept {
            return lhs._offset_ptr <=> rhs._offset_ptr;
        }

    private:
        data_ptr_type _data_ptr{nullptr};
        offset_ptr_type _offset_ptr{nullptr};
    };

    using iterator = segment_iterator<false>;
    using const_iterator = segment_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // --- constructors ---

    segment_vector() = default;

    segment_vector(const segment_vector&) = default;
    segment_vector& operator=(const segment_vector&) = default;

    segment_vector(segment_vector&& other) noexcept
    : _data(std::move(other._data)), _offsets(std::move(other._offsets)) {
        other._offsets = {0uz};
    }

    segment_vector& operator=(segment_vector&& other) noexcept {
        if (this != &other) {
            this->_data = std::move(other._data);
            this->_offsets = std::move(other._offsets);
            other._offsets = {0uz};
        }
        return *this;
    }

    ~segment_vector() = default;

    segment_vector(std::initializer_list<std::initializer_list<value_type>> ilist) {
        this->reserve_segments(ilist.size());

        size_type total_size = 0uz;
        for (const auto& sub : ilist)
            total_size += sub.size();
        this->reserve_data(total_size);

        for (const auto& sub : ilist)
            this->push_back(sub);
    }

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

    friend bool operator==(const segment_vector&, const segment_vector&) = default;

    // --- capacity ---

    [[nodiscard]] size_type size() const noexcept {
        return this->_offsets.size() - 1uz;
    }

    [[nodiscard]] bool empty() const noexcept {
        return this->size() == 0uz;
    }

    [[nodiscard]] size_type segments_capacity() const noexcept {
        return this->_offsets.capacity() - 1uz;
    }

    [[nodiscard]] size_type data_capacity() const noexcept {
        return this->_data.capacity();
    }

    void reserve_segments(size_type n) {
        this->_offsets.reserve(n + 1uz);
    }

    void reserve_data(size_type n) {
        this->_data.reserve(n);
    }

    void shrink_to_fit() {
        this->_data.shrink_to_fit();
        this->_offsets.shrink_to_fit();
    }

    void clear() {
        this->_data.clear();
        this->_offsets.clear();
        this->_offsets.push_back(0uz);
    }

    // --- accessors ---

    [[nodiscard]] segment_type operator[](size_type i) {
        const auto beg = this->_offsets[i];
        const auto end = this->_offsets[i + 1uz];
        return segment_type(this->_data.data() + beg, end - beg);
    }

    [[nodiscard]] const_segment_type operator[](size_type i) const {
        const auto beg = this->_offsets[i];
        const auto end = this->_offsets[i + 1uz];
        return const_segment_type(this->_data.data() + beg, end - beg);
    }

    [[nodiscard]] reference operator[](size_type seg, size_type pos) {
        return this->_data[this->_offsets[seg] + pos];
    }

    [[nodiscard]] const_reference operator[](size_type seg, size_type pos) const {
        return this->_data[this->_offsets[seg] + pos];
    }

    [[nodiscard]] segment_type at(size_type i) {
        this->_check_range(i);
        return (*this)[i];
    }

    [[nodiscard]] const_segment_type at(size_type i) const {
        this->_check_range(i);
        return (*this)[i];
    }

    [[nodiscard]] reference at(size_type seg, size_type pos) {
        this->_check_range(seg);
        this->_check_segment_range(seg, pos);
        return (*this)[seg, pos];
    }

    [[nodiscard]] const_reference at(size_type seg, size_type pos) const {
        this->_check_range(seg);
        this->_check_segment_range(seg, pos);
        return (*this)[seg, pos];
    }

    [[nodiscard]] segment_type front() {
        return (*this)[0uz];
    }

    [[nodiscard]] const_segment_type front() const {
        return (*this)[0uz];
    }

    [[nodiscard]] segment_type back() {
        return (*this)[this->size() - 1uz];
    }

    [[nodiscard]] const_segment_type back() const {
        return (*this)[this->size() - 1uz];
    }

    [[nodiscard]] reference front(size_type seg) {
        return (*this)[seg, 0uz];
    }

    [[nodiscard]] const_reference front(size_type seg) const {
        return (*this)[seg, 0uz];
    }

    [[nodiscard]] reference back(size_type seg) {
        return (*this)[seg, this->segment_size(seg) - 1uz];
    }

    [[nodiscard]] const_reference back(size_type seg) const {
        return (*this)[seg, this->segment_size(seg) - 1uz];
    }

    [[nodiscard]] auto segments() noexcept {
        return std::views::iota(size_type{0}, this->size())
             | std::views::transform([this](size_type i) -> segment_type { return (*this)[i]; });
    }

    [[nodiscard]] auto segments() const noexcept {
        return std::views::iota(size_type{0}, this->size())
             | std::views::transform([this](size_type i) -> const_segment_type {
                   return (*this)[i];
               });
    }

    [[nodiscard]] size_type segment_size(size_type seg) const {
        return this->_offsets[seg + 1uz] - this->_offsets[seg];
    }

    [[nodiscard]] size_type data_size() const noexcept {
        return this->_data.size();
    }

    [[nodiscard]] segment_type data() noexcept {
        return segment_type(this->_data);
    }

    [[nodiscard]] const_segment_type data() const noexcept {
        return const_segment_type(this->_data);
    }

    // --- iterators ---

    [[nodiscard]] iterator begin() noexcept {
        return iterator(this->_data.data(), this->_offsets.data());
    }

    [[nodiscard]] iterator end() noexcept {
        return iterator(this->_data.data(), this->_offsets.data() + this->size());
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return const_iterator(this->_data.data(), this->_offsets.data());
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return const_iterator(this->_data.data(), this->_offsets.data() + this->size());
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

    // --- modifiers (segments) ---

    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void push_back(R&& r) {
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

    void push_back(std::initializer_list<value_type> ilist) {
        this->push_back(std::span<const value_type>{ilist});
    }

    void pop_back() {
        if (this->empty())
            return;

        this->_offsets.pop_back();
        this->_data.resize(this->_offsets.back());
    }

    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void insert(size_type pos, R&& r) {
        const auto beg = this->_offsets[pos];
        const auto old_size = this->_data.size();

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

    void insert(size_type pos, std::initializer_list<value_type> ilist) {
        this->insert(pos, std::span<const value_type>{ilist});
    }

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

    void push_back(size_type seg, const value_type& value) {
        this->insert(seg, this->_offsets[seg + 1uz] - this->_offsets[seg], value);
    }

    template <class... Args>
    void emplace_back(size_type seg, Args&&... args) {
        this->emplace(
            seg, this->_offsets[seg + 1uz] - this->_offsets[seg], std::forward<Args>(args)...
        );
    }

    void pop_back(size_type seg) {
        const auto len = this->_offsets[seg + 1uz] - this->_offsets[seg];
        if (len == 0uz)
            return;
        this->erase(seg, len - 1uz);
    }

    void insert(size_type seg, size_type pos, const value_type& value) {
        this->_data.insert(this->_data.begin() + this->_offsets[seg] + pos, value);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]++;
    }

    template <class... Args>
    void emplace(size_type seg, size_type pos, Args&&... args) {
        this->_data.emplace(
            this->_data.begin() + this->_offsets[seg] + pos, std::forward<Args>(args)...
        );
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]++;
    }

    void erase(size_type seg, size_type pos) {
        this->_data.erase(this->_data.begin() + this->_offsets[seg] + pos);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]--;
    }

private:
    void _check_range(size_type n) const {
        if (n >= this->size())
            throw std::out_of_range(std::format(
                "segment_vector::_check_range: n (which is {}) >= this->size() (which is {})",
                n,
                this->size()
            ));
    }

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

    std::vector<value_type> _data;
    std::vector<size_type> _offsets{0uz};
};
