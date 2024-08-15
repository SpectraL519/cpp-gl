#pragma once

#include "type_traits.hpp"
#include "types.hpp"

#include <format>
#include <iterator>

namespace gl {

namespace types {

template <std::forward_iterator Iterator>
class iterator_range {
public:
    using iterator = Iterator;
#if __cplusplus >= 202302L
    using const_iterator = std::const_iterator<Iterator>;
#endif
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_reference_t<typename iterator::value_type>;

    iterator_range() = delete;

    explicit iterator_range(iterator begin, iterator end) : _range(begin, end) {}

    template <type_traits::c_range Range>
    explicit iterator_range(Range& range)
    : _range(std::ranges::begin(range), std::ranges::end(range)) {}

    iterator_range(const iterator_range&) = default;
    iterator_range(iterator_range&&) = default;

    iterator_range& operator=(const iterator_range&) = default;
    iterator_range& operator=(iterator_range&&) = default;

    ~iterator_range() = default;

    bool operator==(const iterator_range&) const = default;

    [[nodiscard]] inline iterator begin() const {
        return this->_range.first;
    }

    [[nodiscard]] inline iterator end() const {
        return this->_range.second;
    }

#if __cplusplus >= 202302L
    [[nodiscard]] inline auto cbegin() const {
        return std::make_const_iterator(this->_range.first);
    }

    [[nodiscard]] inline auto cend() const {
        return std::make_const_iterator(this->_range.second);
    }
#endif

    [[nodiscard]] inline difference_type distance() const {
        return std::ranges::distance(this->begin(), this->end());
    }

    [[nodiscard]] value_type& element_at(types::size_type n) const {
        const auto distance = this->distance();
        if (not (n < this->distance()))
            throw std::out_of_range(
                std::format("Position index {} out of range [0, {}]", n, this->distance())
            );
        return *std::ranges::next(this->begin(), n);
    }

    // TODO: validate begin <(=?) end
    // If validated, use size_type instead of ptrdiff_t

    inline void advance_begin(difference_type n = _default_n) {
        std::ranges::advance(this->_range.first, n);
    }

    inline void advance_end(difference_type n = _default_n) {
        std::ranges::advance(this->_range.second, n);
    }

private:
    homogeneous_pair<iterator> _range;

    static constexpr difference_type _default_n = 1;
};

} // namespace types

template <std::forward_iterator Iterator>
[[nodiscard]] inline types::iterator_range<Iterator> make_iterator_range(
    Iterator begin, Iterator end
) {
    return types::iterator_range{begin, end};
}

template <type_traits::c_range Range>
[[nodiscard]] inline types::iterator_range<type_traits::iterator_type<Range>> make_iterator_range(
    Range& range
) {
    return types::iterator_range{std::ranges::begin(range), std::ranges::end(range)};
}

template <type_traits::c_range Range>
[[nodiscard]] inline types::iterator_range<type_traits::const_iterator_type<Range>>
make_const_iterator_range(Range& range) {
    return types::iterator_range{std::ranges::cbegin(range), std::ranges::cend(range)};
}

} // namespace gl
