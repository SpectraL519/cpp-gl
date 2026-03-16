#pragma once

#include <concepts>
#include <format>
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>
#include <cstdint>

template <std::semiregular T>
class segmented_vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using segment_type = std::span<T>;
    using const_segment_type = std::span<const T>;

    segmented_vector() = default;

    // TODO: copy/move ctors

    // ----- capacity -----

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

    void reserve_values(size_type n) {
        this->_data.reserve(n);
    }

    void shrink_to_fit() {
        this->_data.shrink_to_fit();
        this->_offsets.shrink_to_fit();
    }

    // ----- accessors -----

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

    [[nodiscard]] segment_type at(size_type i) {
        this->_check_range(i);
        return (*this)[i];
    }

    [[nodiscard]] const_segment_type at(size_type i) const {
        this->_check_range(i);
        return (*this)[i];
    }

    [[nodiscard]] auto segments() noexcept {
        return std::views::iota(size_type{0}, size())
            | std::views::transform([this](size_type i) -> segment_type {
                return (*this)[i];
            });
    }

    [[nodiscard]] auto segments() const noexcept {
        return std::views::iota(size_type{0}, size())
            | std::views::transform([this](size_type i) -> const_segment_type {
                return (*this)[i];
            });
    }

    // flatten -> data ???

    [[nodiscard]] segment_type flatten() noexcept {
        return segment_type(this->_data);
    }

    [[nodiscard]] const_segment_type flatten() const noexcept {
        return const_segment_type(this->_data);
    }

    // ----- modifiers (segments) -----

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

    // TODO: emplace_back

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

    // TODO: emplace

    void erase(size_type pos) {
        const auto start = this->_offsets[pos];
        const auto end = this->_offsets[pos + 1uz];
        const auto len = end - start;

        this->_data.erase(this->_data.begin() + start, this->_data.begin() + end);
        this->_offsets.erase(this->_offsets.begin() + pos);
        for (size_type i = pos; i < this->_offsets.size(); i++)
            this->_offsets[i] -= len;
    }

    // ----- modifiers (elements) -----

    void push_back(size_type seg, const T& value) {
        this->insert(seg, this->_offsets[seg + 1uz] - this->_offsets[seg], value);
    }

    // TODO: emplace_back

    void pop_back(size_type seg) {
        this->erase(seg, this->_offsets[seg + 1uz] - this->_offsets[seg] - 1uz);
    }

    void insert(size_type seg, size_type pos, const value_type& value) {
        this->_data.insert(this->_data.begin() + this->_offsets[seg] + pos, value);
        for (size_type i = seg + 1uz; i < this->_offsets.size(); i++)
            this->_offsets[i]++;
    }

    // TODO: emplace_back

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

    std::vector<T> _data;
    std::vector<size_type> _offsets{0uz};
};
