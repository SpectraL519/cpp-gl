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
class segmented_vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using segment_type = std::span<T>;
    using const_segment_type = std::span<const T>;

    // --- constructors ---

    segmented_vector() = default;

    segmented_vector(const segmented_vector&) = default;
    segmented_vector& operator=(const segmented_vector&) = default;

    segmented_vector(segmented_vector&& other) noexcept
    : _data(std::move(other._data)), _offsets(std::move(other._offsets)) {
        other._offsets = {0uz};
    }

    segmented_vector& operator=(segmented_vector&& other) noexcept {
        if (this != &other) {
            this->_data = std::move(other._data);
            this->_offsets = std::move(other._offsets);
            other._offsets = {0uz};
        }
        return *this;
    }

    ~segmented_vector() = default;

    segmented_vector(std::initializer_list<std::initializer_list<T>> ilist) {
        this->reserve_segments(ilist.size());

        size_type total_size = 0uz;
        for (const auto& sub : ilist)
            total_size += sub.size();
        this->reserve_data(total_size);

        for (const auto& sub : ilist)
            this->push_back(sub);
    }

    template <std::ranges::input_range R>
    requires std::ranges::input_range<std::ranges::range_reference_t<R>> and
             std::convertible_to<std::ranges::range_reference_t<std::ranges::range_reference_t<R>>, value_type>
    explicit segmented_vector(R&& r) {
        if constexpr (std::ranges::sized_range<R>)
            this->reserve_segments(std::ranges::size(r));

        for (auto&& subrange : r)
            this->push_back(std::forward<decltype(subrange)>(subrange));
    }

    // --- comparsion ---

    friend bool operator==(const segmented_vector&, const segmented_vector&) = default;

    // --- capacity ---

    [[nodiscard]] size_type size() const noexcept {
        return this->_offsets.size() - 1uz;
    }

    [[nodiscard]] bool empty() const noexcept {
        return this->size() == 0uz;
    }

    void clear() {
        this->_data.clear();
        this->_offsets.clear();
        this->_offsets.push_back(0uz);
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

    [[nodiscard]] auto segments() noexcept {
        return std::views::iota(size_type{0}, this->size())
            | std::views::transform([this](size_type i) -> segment_type {
                return (*this)[i];
            });
    }

    [[nodiscard]] size_type segment_size(size_type seg) const {
        return this->_offsets[seg + 1uz] - this->_offsets[seg];
    }

    [[nodiscard]] segment_type flatten() noexcept {
        return segment_type(this->_data);
    }

    [[nodiscard]] const_segment_type flatten() const noexcept {
        return const_segment_type(this->_data);
    }

    // --- modifiers (segments) ---

    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    void push_back(R&& r) {
        this->_offsets.reserve(this->_offsets.size() + 1uz);
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
                this->_data.begin() + beg,
                std::ranges::begin(r),
                std::ranges::end(r)
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

    void push_back(size_type seg, const T& value) {
        this->insert(seg, this->_offsets[seg + 1uz] - this->_offsets[seg], value);
    }

    template <class... Args>
    void emplace_back(size_type seg, Args&&... args) {
        this->emplace(seg, this->_offsets[seg + 1uz] - this->_offsets[seg], std::forward<Args>(args)...);
    }

    void pop_back(size_type seg) {
        const auto len = this->_offsets[seg + 1uz] - this->_offsets[seg];
        if (len == 0uz) return;
        this->erase(seg, len - 1uz);
    }

    void insert(size_type seg, size_type pos, const value_type& value) {
        this->_data.insert(this->_data.begin() + this->_offsets[seg] + pos, value);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]++;
    }

    template <class... Args>
    void emplace(size_type seg, size_type pos, Args&&... args) {
        this->_data.emplace(this->_data.begin() + this->_offsets[seg] + pos, std::forward<Args>(args)...);
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
                "segmented_vector::_check_range: n (which is {}) >= this->size() (which is {})",
                n,
                this->size()
            ));
    }

    void _check_segment_range(size_type seg, size_type pos) const {
        if (pos >= this->segment_size(seg)) {
            throw std::out_of_range(std::format(
                "segmented_vector::_check_segment_range: pos (which is {}) >= segment_size({}) (which is {})",
                pos,
                seg,
                this->segment_size(seg)
            ));
        }
    }

    std::vector<T> _data;
    std::vector<size_type> _offsets{0uz};
};
