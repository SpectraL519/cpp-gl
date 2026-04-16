#include "doctest.h"

#include <gl/attributes/diagnostics.hpp>
#include <gl/types/core.hpp>
#include <gl/types/flat_jagged_vector.hpp>

#include <algorithm>
#include <functional>
#include <ranges>
#include <utility>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_flat_jagged_vector");

struct test_flat_jagged_vector_constructors {
    using sut_type = gl::flat_jagged_vector<int>;
};

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors,
    "default constructor should create empty flat_jagged_vector"
) {
    sut_type sut;
    CHECK(sut.empty());
    CHECK_EQ(sut.size(), 0uz);
    CHECK_EQ(sut.data_size(), 0uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors, "copy constructor should create an equal copy"
) {
    sut_type original;
    original.push_back({1, 2, 3});
    original.push_back({4, 5});

    sut_type copy = original;

    CHECK_EQ(copy, original);
    CHECK_EQ(copy.size(), 2uz);
    CHECK_EQ(copy.data_size(), 5uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors, "copy assignment should create an equal copy"
) {
    sut_type original;
    original.push_back({1, 2, 3});
    original.push_back({4, 5});

    sut_type target;
    target = original;

    CHECK_EQ(target, original);
    CHECK_EQ(target.size(), 2uz);
    CHECK_EQ(target.data_size(), 5uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors, "move constructor should transfer ownership"
) {
    sut_type source;
    source.push_back({1, 2, 3});
    source.push_back({4, 5});

    sut_type dest = std::move(source);

    CHECK_EQ(dest.size(), 2uz);
    CHECK_EQ(dest.data_size(), 5uz);
    CHECK(source.empty());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors, "move assignment should transfer ownership"
) {
    sut_type source;
    source.push_back({1, 2, 3});
    source.push_back({4, 5});

    sut_type target;
    target = std::move(source);

    CHECK_EQ(target.size(), 2uz);
    CHECK_EQ(target.data_size(), 5uz);
    CHECK(source.empty());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors, "move assignment should handle self-assignment correctly"
) {
    sut_type sut;
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    GL_SUPPRESS_WARNING_BEGIN("-Wself-move");
    sut = std::move(sut);
    GL_SUPPRESS_WARNING_END;

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors, "(n_segments) constructor should initialize segments"
) {
    sut_type sut(3uz);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), 0uz);
    for (std::size_t i = 0uz; i < 3uz; ++i)
        CHECK(sut.empty(i));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors,
    "(n_segments, segment_size) constructor should initialize segments"
) {
    sut_type sut(3uz, 5uz);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), 15uz);
    for (std::size_t i = 0uz; i < 3uz; ++i) {
        CHECK_EQ(sut.segment_size(i), 5uz);
        for (std::size_t j = 0uz; j < 5uz; ++j)
            CHECK_EQ(sut[i, j], 0);
    }
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors, "initializer list constructor should initialize segments"
) {
    sut_type sut{
        {1, 2, 3},
        {4, 5},
        {6}
    };

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{6}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_constructors, "range constructor should initialize from nested ranges"
) {
    std::vector<std::vector<int>> data{
        {1, 2, 3},
        {4, 5},
        {6}
    };
    sut_type sut(data);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::equal(sut[0uz], data[0uz]));
    CHECK(std::ranges::equal(sut[1uz], data[1uz]));
    CHECK(std::ranges::equal(sut[2uz], data[2uz]));
}

struct test_flat_jagged_vector_comparison {
    using sut_type = gl::flat_jagged_vector<int>;
};

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_comparison,
    "equality operator should return true for equal flat_jagged_vectors"
) {
    sut_type jv1{
        {1, 2},
        {3, 4}
    };
    sut_type jv2{
        {1, 2},
        {3, 4}
    };

    CHECK_EQ(jv1, jv2);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_comparison,
    "inequality operator should return true for different flat_jagged_vectors"
) {
    sut_type jv1{
        {1, 2},
        {3, 4}
    };
    sut_type jv2{
        {1, 2},
        {3, 5}
    };
    sut_type jv3{
        {1, 2},
        {3, 4},
        {5}
    };

    CHECK_NE(jv1, jv2);
    CHECK_NE(jv1, jv3);
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_comparison, "empty flat_jagged_vectors should be equal") {
    sut_type jv1;
    sut_type jv2;

    CHECK_EQ(jv1, jv2);
}

struct test_flat_jagged_vector_capacity {
    using sut_type = gl::flat_jagged_vector<int>;
    sut_type sut;
};

TEST_CASE_FIXTURE(test_flat_jagged_vector_capacity, "size should return the number of segments") {
    CHECK_EQ(sut.size(), 0uz);

    sut.push_back({1, 2, 3});
    CHECK_EQ(sut.size(), 1uz);

    sut.push_back({4, 5});
    CHECK_EQ(sut.size(), 2uz);

    sut.push_back({6});
    CHECK_EQ(sut.size(), 3uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "empty should return true only when there are no segments"
) {
    CHECK(sut.empty());

    sut.push_back({1});
    CHECK_FALSE(sut.empty());

    sut.pop_back();
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "reserve_segments should reserve space for segments"
) {
    sut.reserve_segments(10uz);
    CHECK_EQ(sut.segments_capacity(), 10uz);
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_capacity, "reserve_data should reserve space for data") {
    sut.reserve_data(100uz);
    CHECK_EQ(sut.data_capacity(), 100uz);
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_capacity, "shrink_to_fit should reduce capacity") {
    sut.push_back({1, 2, 3});
    sut.shrink_to_fit();

    CHECK_EQ(sut.segments_capacity(), 1uz);
    CHECK_EQ(sut.data_capacity(), 3uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "resize(n) should shrink container when n < size"
) {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});
    sut.push_back({6, 7, 8});

    sut.resize(2uz);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "resize(n) should grow container when n > size"
) {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    sut.resize(4uz);

    CHECK_EQ(sut.size(), 4uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
    CHECK(sut[2uz].empty());
    CHECK(sut[3uz].empty());
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_capacity, "resize(n) should do nothing when n == size") {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    sut.resize(2uz);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "resize(n, range) should shrink container when n < size"
) {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});
    sut.push_back({6, 7, 8});

    std::vector<int> new_seg{9, 10};
    sut.resize(2uz, new_seg);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "resize(n, range) should grow container when n > size"
) {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    std::vector<int> new_seg{6, 7};
    sut.resize(4uz, new_seg);

    CHECK_EQ(sut.size(), 4uz);
    CHECK_EQ(sut.data_size(), 5uz + 2uz * 2uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
    CHECK(std::ranges::equal(sut[2uz], new_seg));
    CHECK(std::ranges::equal(sut[3uz], new_seg));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "resize(n, range) should do nothing when n == size"
) {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    std::vector<int> new_seg{6, 7};
    sut.resize(2uz, new_seg);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "resize(n, initializer_list) should grow container"
) {
    sut.push_back({1, 2, 3});

    sut.resize(3uz, {4, 5});

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), 3uz + 2uz * 2uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_capacity, "resize(n, initializer_list) should shrink container"
) {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});
    sut.push_back({6, 7, 8});

    sut.resize(2uz, {9, 10});

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_capacity, "clear should remove all segments and data") {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    sut.clear();

    CHECK(sut.empty());
    CHECK_EQ(sut.data_size(), 0uz);
}

struct test_flat_jagged_vector_accessors {
    using sut_type = gl::flat_jagged_vector<int>;

    sut_type sut{
        {1, 2, 3},
        {4, 5},
        {6}
    };

    std::vector<std::vector<int>> data{
        {1, 2, 3},
        {4, 5},
        {6}
    };
    std::vector<int> flat_data{1, 2, 3, 4, 5, 6};
    std::vector<std::size_t> offsets{0uz, 3uz, 5uz, 6uz};

    int dummy_value = 111;
    std::size_t dummy_offset = 999uz;
};

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "operator[] should return segment at given index"
) {
    auto seg0 = sut[0uz];
    CHECK(std::ranges::equal(seg0, data[0uz]));
    seg0.front() = dummy_value;
    CHECK_EQ(sut[0uz].front(), dummy_value);

    auto seg1 = sut[1uz];
    CHECK(std::ranges::equal(seg1, data[1uz]));
    seg1.front() = dummy_value;
    CHECK_EQ(sut[1uz].front(), dummy_value);

    auto seg2 = sut[2uz];
    CHECK(std::ranges::equal(seg2, data[2uz]));
    seg2.front() = dummy_value;
    CHECK_EQ(sut[2uz].front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "const operator[] should return const segment at given index"
) {
    const auto& const_sut = sut;

    auto seg0 = const_sut[0uz];
    CHECK(std::ranges::equal(seg0, data[0uz]));

    auto seg1 = const_sut[1uz];
    CHECK(std::ranges::equal(seg1, data[1uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_accessors, "at() should return segment at given index") {
    auto seg0 = sut.at(0uz);
    CHECK(std::ranges::equal(seg0, data[0uz]));
    seg0.front() = dummy_value;
    CHECK_EQ(sut.at(0uz).front(), dummy_value);

    auto seg1 = sut.at(1uz);
    CHECK(std::ranges::equal(seg1, data[1uz]));
    seg1.front() = dummy_value;
    CHECK_EQ(sut.at(1uz).front(), dummy_value);

    auto seg2 = sut.at(2uz);
    CHECK(std::ranges::equal(seg2, data[2uz]));
    seg2.front() = dummy_value;
    CHECK_EQ(sut.at(2uz).front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_accessors, "at() should throw for out of range index") {
    CHECK_THROWS_AS(static_cast<void>(sut.at(3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "const at() should return const segment at given index"
) {
    const auto& const_sut = sut;

    auto seg0 = const_sut.at(0uz);
    CHECK(std::ranges::equal(seg0, data[0uz]));

    auto seg1 = const_sut.at(1uz);
    CHECK(std::ranges::equal(seg1, data[1uz]));

    auto seg2 = const_sut.at(2uz);
    CHECK(std::ranges::equal(seg2, data[2uz]));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "const at() should throw for out of range index"
) {
    const auto& const_sut = sut;
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "segments() should return a view of all segments"
) {
    auto n_segments = 0uz;
    for (auto seg : sut.segments()) {
        CHECK(std::ranges::equal(seg, data[n_segments]));
        const auto orig_val = std::exchange(seg.front(), dummy_value);
        CHECK_EQ(sut[n_segments].front(), dummy_value);
        seg.front() = orig_val; // revert change

        n_segments++;
    }

    CHECK_EQ(n_segments, 3uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "const segments() should return a const view of all segments"
) {
    const auto& const_sut = sut;

    auto n_segments = 0uz;
    for (auto seg : const_sut.segments()) {
        CHECK(std::ranges::equal(seg, data[n_segments]));
        n_segments++;
    }

    CHECK_EQ(n_segments, 3uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "empty(i) should return true for empty segments and false for non-empty segments"
) {
    CHECK_FALSE(sut.empty(0uz));
    CHECK_FALSE(sut.empty(1uz));
    CHECK_FALSE(sut.empty(2uz));

    sut.push_back({});
    CHECK(sut.empty(3uz));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "segment_size(i) should return the size of a segment"
) {
    CHECK_EQ(sut.segment_size(0uz), 3uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut.segment_size(2uz), 1uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "data_size() should return the total number of elements"
) {
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "data_view() should return a span of all data"
) {
    CHECK(std::ranges::equal(sut.data_view(), flat_data));
    sut.data_view().front() = dummy_value;
    CHECK_EQ(sut.data_view().front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "const data_view() should return a const span of all data"
) {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.data_view(), flat_data));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "data_storage() should return a mutable reference to the internal vector"
) {
    auto& storage_ref = sut.data_storage();
    CHECK(std::ranges::equal(storage_ref, flat_data));
    CHECK_EQ(storage_ref.data(), sut.data_view().data());

    // check mutability
    storage_ref.front() = dummy_value;
    CHECK_EQ(sut.data_view().front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "const data_storage() should return a const reference to the internal vector"
) {
    const auto& const_sut = sut;
    const auto& storage_ref = const_sut.data_storage();

    CHECK(std::ranges::equal(storage_ref, flat_data));
    CHECK_EQ(storage_ref.data(), const_sut.data_view().data());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "data_ptr() should return a mutable raw pointer to the first element"
) {
    auto* ptr = sut.data_ptr();

    CHECK_EQ(ptr, sut.data_view().data());
    CHECK(std::equal(ptr, ptr + sut.data_size(), flat_data.begin()));

    // check mutability
    *ptr = dummy_value;
    CHECK_EQ(sut.data_view().front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "const data_ptr() should return a const raw pointer to the first element"
) {
    const auto& const_sut = sut;
    const auto* ptr = const_sut.data_ptr();

    CHECK_EQ(ptr, const_sut.data_view().data());
    CHECK(std::equal(ptr, ptr + const_sut.data_size(), flat_data.begin()));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "offsets_view() should return a span of all offsets"
) {
    CHECK(std::ranges::equal(sut.offsets_view(), offsets));
    sut.offsets_view().front() = dummy_offset;
    CHECK_EQ(sut.offsets_view().front(), dummy_offset);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "const offsets_view() should return a const span of all offsets"
) {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.offsets_view(), offsets));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "offsets_storage() should return a mutable reference to the internal vector"
) {
    auto& storage_ref = sut.offsets_storage();
    CHECK(std::ranges::equal(storage_ref, offsets));
    CHECK_EQ(storage_ref.data(), sut.offsets_view().data());

    // check mutability
    storage_ref.front() = dummy_offset;
    CHECK_EQ(sut.offsets_view().front(), dummy_offset);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "const offsets_storage() should return a const reference to the internal vector"
) {
    const auto& const_sut = sut;
    const auto& storage_ref = const_sut.offsets_storage();

    CHECK(std::ranges::equal(storage_ref, offsets));
    CHECK_EQ(storage_ref.data(), const_sut.offsets_view().data());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "offsets_ptr() should return a mutable raw pointer to the first element"
) {
    auto* ptr = sut.offsets_ptr();

    CHECK_EQ(ptr, sut.offsets_view().data());
    CHECK(std::equal(ptr, ptr + sut.offsets_view().size(), offsets.begin()));

    // check mutability
    *ptr = dummy_offset;
    CHECK_EQ(sut.offsets_view().front(), dummy_offset);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors,
    "const offsets_ptr() should return a const raw pointer to the first element"
) {
    const auto& const_sut = sut;
    const auto* ptr = const_sut.offsets_ptr();

    CHECK_EQ(ptr, const_sut.offsets_view().data());
    CHECK(std::equal(ptr, ptr + const_sut.offsets_view().size(), offsets.begin()));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_accessors, "front() should return the first segment") {
    auto front_seg = sut.front();
    CHECK(std::ranges::equal(front_seg, data.front()));

    front_seg.front() = dummy_value;
    CHECK_EQ(sut.front().front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "const front() should return const first segment"
) {
    const auto& const_sut = sut;
    auto front_seg = const_sut.front();
    CHECK(std::ranges::equal(front_seg, data.front()));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_accessors, "back() should return the last segment") {
    auto back_seg = sut.back();
    CHECK(std::ranges::equal(back_seg, data.back()));

    back_seg.front() = dummy_value;
    CHECK_EQ(sut.back().front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_accessors, "const back() should return const last segment"
) {
    const auto& const_sut = sut;
    auto back_seg = const_sut.back();
    CHECK(std::ranges::equal(back_seg, data.back()));
}

struct test_flat_jagged_vector_element_accessors {
    using sut_type = gl::flat_jagged_vector<int>;

    std::vector<int> seg0{1, 2, 3};
    std::vector<int> seg1{4, 5};
    std::vector<int> seg2{6};
    sut_type sut{
        std::vector<std::vector<int>>{seg0, seg1, seg2}
    };

    int dummy_value = 111;
};

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors,
    "operator[](seg, pos) should return element at given segment and position"
) {
    CHECK_EQ(sut[0uz, 0uz], seg0[0uz]);
    sut[0uz, 0uz] = dummy_value;
    CHECK_EQ(sut[0uz, 0uz], dummy_value);

    CHECK_EQ(sut[0uz, 1uz], seg0[1uz]);
    sut[0uz, 1uz] = dummy_value;
    CHECK_EQ(sut[0uz, 1uz], dummy_value);

    CHECK_EQ(sut[0uz, 2uz], seg0[2uz]);
    sut[0uz, 2uz] = dummy_value;
    CHECK_EQ(sut[0uz, 2uz], dummy_value);

    CHECK_EQ(sut[1uz, 0uz], seg1[0uz]);
    sut[1uz, 0uz] = dummy_value;
    CHECK_EQ(sut[1uz, 0uz], dummy_value);

    CHECK_EQ(sut[1uz, 1uz], seg1[1uz]);
    sut[1uz, 1uz] = dummy_value;
    CHECK_EQ(sut[1uz, 1uz], dummy_value);

    CHECK_EQ(sut[2uz, 0uz], seg2[0uz]);
    sut[2uz, 0uz] = dummy_value;
    CHECK_EQ(sut[2uz, 0uz], dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors,
    "const operator[](seg, pos) should return const element"
) {
    const auto& const_sut = sut;

    CHECK_EQ(const_sut[0uz, 0uz], seg0[0uz]);
    CHECK_EQ(const_sut[0uz, 1uz], seg0[1uz]);
    CHECK_EQ(const_sut[0uz, 2uz], seg0[2uz]);
    CHECK_EQ(const_sut[1uz, 0uz], seg1[0uz]);
    CHECK_EQ(const_sut[1uz, 1uz], seg1[1uz]);
    CHECK_EQ(const_sut[2uz, 0uz], seg2[0uz]);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors,
    "at(seg, pos) should return element at given segment and position"
) {
    CHECK_EQ(sut.at(0uz, 0uz), seg0[0uz]);
    sut.at(0uz, 0uz) = dummy_value;
    CHECK_EQ(sut.at(0uz, 0uz), dummy_value);

    CHECK_EQ(sut.at(0uz, 1uz), seg0[1uz]);
    sut.at(0uz, 1uz) = dummy_value;
    CHECK_EQ(sut.at(0uz, 1uz), dummy_value);

    CHECK_EQ(sut.at(0uz, 2uz), seg0[2uz]);
    sut.at(0uz, 2uz) = dummy_value;
    CHECK_EQ(sut.at(0uz, 2uz), dummy_value);

    CHECK_EQ(sut.at(1uz, 0uz), seg1[0uz]);
    sut.at(1uz, 0uz) = dummy_value;
    CHECK_EQ(sut.at(1uz, 0uz), dummy_value);

    CHECK_EQ(sut.at(1uz, 1uz), seg1[1uz]);
    sut.at(1uz, 1uz) = dummy_value;
    CHECK_EQ(sut.at(1uz, 1uz), dummy_value);

    CHECK_EQ(sut.at(2uz, 0uz), seg2[0uz]);
    sut.at(2uz, 0uz) = dummy_value;
    CHECK_EQ(sut.at(2uz, 0uz), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors, "at(seg, pos) should throw for invalid segment"
) {
    CHECK_THROWS_AS(static_cast<void>(sut.at(3uz, 0uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors, "at(seg, pos) should throw for invalid position"
) {
    CHECK_THROWS_AS(static_cast<void>(sut.at(0uz, 3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(1uz, 2uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(2uz, 1uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors, "const at(seg, pos) should return const element"
) {
    const auto& const_sut = sut;

    CHECK_EQ(const_sut.at(0uz, 0uz), seg0[0uz]);
    CHECK_EQ(const_sut.at(0uz, 1uz), seg0[1uz]);
    CHECK_EQ(const_sut.at(0uz, 2uz), seg0[2uz]);
    CHECK_EQ(const_sut.at(1uz, 0uz), seg1[0uz]);
    CHECK_EQ(const_sut.at(1uz, 1uz), seg1[1uz]);
    CHECK_EQ(const_sut.at(2uz, 0uz), seg2[0uz]);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors, "const at(seg, pos) should throw for invalid segment"
) {
    const auto& const_sut = sut;

    CHECK_THROWS_AS(static_cast<void>(const_sut.at(3uz, 0uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors,
    "const at(seg, pos) should throw for invalid position"
) {
    const auto& const_sut = sut;

    CHECK_THROWS_AS(static_cast<void>(const_sut.at(0uz, 3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(1uz, 2uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(2uz, 1uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors, "front(seg) should return first element in segment"
) {
    CHECK_EQ(sut.front(0uz), seg0.front());
    sut.front(0uz) = dummy_value;
    CHECK_EQ(sut[0uz].front(), dummy_value);

    CHECK_EQ(sut.front(1uz), seg1.front());
    sut.front(1uz) = dummy_value;
    CHECK_EQ(sut[1uz].front(), dummy_value);

    CHECK_EQ(sut.front(2uz), seg2.front());
    sut.front(2uz) = dummy_value;
    CHECK_EQ(sut[2uz].front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors,
    "const front(seg) should return const first element in segment"
) {
    const auto& const_sut = sut;
    CHECK_EQ(const_sut.front(0uz), seg0.front());
    CHECK_EQ(const_sut.front(1uz), seg1.front());
    CHECK_EQ(const_sut.front(2uz), seg2.front());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors, "back(seg) should return last element in segment"
) {
    CHECK_EQ(sut.back(0uz), seg0.back());
    sut.back(0uz) = dummy_value;
    CHECK_EQ(sut[0uz].back(), dummy_value);

    CHECK_EQ(sut.back(1uz), seg1.back());
    sut.back(1uz) = dummy_value;
    CHECK_EQ(sut[1uz].back(), dummy_value);

    CHECK_EQ(sut.back(2uz), seg2.back());
    sut.back(2uz) = dummy_value;
    CHECK_EQ(sut[2uz].back(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_accessors,
    "const back(seg) should return const last element in segment"
) {
    const auto& const_sut = sut;
    CHECK_EQ(const_sut.back(0uz), seg0.back());
    CHECK_EQ(const_sut.back(1uz), seg1.back());
    CHECK_EQ(const_sut.back(2uz), seg2.back());
}

struct test_flat_jagged_vector_segment_modifiers {
    using sut_type = gl::flat_jagged_vector<int>;

    sut_type sut;
    std::vector<int> seg0{1, 2, 3};
    std::vector<int> seg1{4, 5};
    std::vector<int> seg2{6, 7, 8};
};

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers, "push_back with span should add new segment"
) {
    sut.push_back(std::span<const int>{seg0});

    CHECK_EQ(sut.size(), 1uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut[0uz], seg0));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers, "push_back with vector should add new segment"
) {
    sut.push_back(seg0);

    CHECK_EQ(sut.size(), 1uz);
    CHECK(std::ranges::equal(sut[0uz], seg0));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers,
    "push_back with initializer list should add new segment"
) {
    sut.push_back({1, 2, 3});

    CHECK_EQ(sut.size(), 1uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers,
    "multiple push_back calls should add multiple segments"
) {
    sut.push_back(seg0);
    sut.push_back(seg1);
    sut.push_back(seg2);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), seg0.size() + seg1.size() + seg2.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
    CHECK(std::ranges::equal(sut[1uz], seg1));
    CHECK(std::ranges::equal(sut[2uz], seg2));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_segment_modifiers, "pop_back should remove last segment") {
    sut.push_back(seg0);
    sut.push_back(seg1);

    sut.pop_back();

    CHECK_EQ(sut.size(), 1uz);
    CHECK_EQ(sut.data_size(), seg0.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers, "pop_back on empty container should do nothing"
) {
    sut.pop_back();
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers, "pop_back should remove all segments sequentially"
) {
    sut.push_back(seg0);
    sut.push_back(seg1);
    sut.push_back(seg2);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), seg0.size() + seg1.size() + seg2.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
    CHECK(std::ranges::equal(sut[1uz], seg1));
    CHECK(std::ranges::equal(sut[2uz], seg2));

    sut.pop_back();
    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), seg0.size() + seg1.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
    CHECK(std::ranges::equal(sut[1uz], seg1));

    sut.pop_back();
    CHECK_EQ(sut.size(), 1uz);
    CHECK_EQ(sut.data_size(), seg0.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));

    sut.pop_back();
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers, "insert should add segment at given position"
) {
    sut.push_back(seg0);
    sut.push_back(seg2);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), seg0.size() + seg2.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
    CHECK(std::ranges::equal(sut[1uz], seg2));

    sut.insert(1uz, seg1);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), seg0.size() + seg1.size() + seg2.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
    CHECK(std::ranges::equal(sut[1uz], seg1));
    CHECK(std::ranges::equal(sut[2uz], seg2));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers, "erase should remove segment at given position"
) {
    sut.push_back(seg0);
    sut.push_back(seg1);
    sut.push_back(seg2);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), seg0.size() + seg1.size() + seg2.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
    CHECK(std::ranges::equal(sut[1uz], seg1));
    CHECK(std::ranges::equal(sut[2uz], seg2));

    sut.erase(1);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), seg0.size() + seg2.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
    CHECK(std::ranges::equal(sut[1uz], seg2));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_segment_modifiers, "erase should update offsets correctly"
) {
    sut.push_back(seg0);
    sut.push_back(seg1);
    sut.push_back(seg2);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), seg0.size() + seg1.size() + seg2.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
    CHECK(std::ranges::equal(sut[1uz], seg1));
    CHECK(std::ranges::equal(sut[2uz], seg2));

    sut.erase(0uz);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut[0uz].front(), seg1[0uz]);
    CHECK_EQ(sut[1uz].front(), seg2[0uz]);
}

struct test_flat_jagged_vector_element_modifiers {
    using sut_type = gl::flat_jagged_vector<int>;
    sut_type sut{
        {1, 2, 3},
        {4, 5}
    };
};

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers, "push_back element should add element to segment"
) {
    sut.push_back(0uz, 10);

    CHECK_EQ(sut[0uz].back(), 10);
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers, "emplace_back should add element to segment"
) {
    sut.emplace_back(0uz, 10);

    CHECK_EQ(sut[0uz].back(), 10);
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "pop_back element should remove last element from segment"
) {
    sut.pop_back(0uz);

    CHECK_EQ(sut.segment_size(0uz), 2uz);
    CHECK_EQ(sut.data_size(), 4uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers, "pop_back on empty segment should do nothing"
) {
    sut_type empty_sv;
    empty_sv.push_back({});
    empty_sv.pop_back(0uz);

    CHECK_EQ(empty_sv.segment_size(0uz), 0uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "insert element should add element at position in segment"
) {
    sut.insert(0uz, 1uz, 10);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 10, 2, 3}));
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers, "emplace should add element at position in segment"
) {
    sut.emplace(0uz, 1uz, 10);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 10, 2, 3}));
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "erase element should remove element at position in segment"
) {
    sut.erase(0uz, 1uz);

    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
    CHECK_EQ(sut.data_size(), 4uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "insert element should update offsets for other segments"
) {
    sut.insert(0uz, 0uz, 0);

    CHECK_EQ(sut.segment_size(0uz), 4uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut[1uz].front(), 4);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "erase element should update offsets for other segments"
) {
    sut.erase(0uz, 0uz);

    CHECK_EQ(sut.segment_size(0uz), 2uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut[0uz].front(), 2);
    CHECK_EQ(sut[1uz].front(), 4);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "resize(seg, n) should shrink segment when n < segment_size"
) {
    sut.resize(0uz, 2uz);

    CHECK_EQ(sut.segment_size(0uz), 2uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut.data_size(), 4uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "resize(seg, n) should grow segment when n > segment_size"
) {
    sut.resize(0uz, 5uz);

    CHECK_EQ(sut.segment_size(0uz), 5uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut.data_size(), 7uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3, 0, 0}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "resize(seg, n) should do nothing when n == segment_size"
) {
    sut.resize(0uz, 3uz);

    CHECK_EQ(sut.segment_size(0uz), 3uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "resize(seg, n, value) should shrink segment when n < segment_size"
) {
    sut.resize(0uz, 1uz, 99);

    CHECK_EQ(sut.segment_size(0uz), 1uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "resize(seg, n, value) should grow segment when n > segment_size"
) {
    sut.resize(0uz, 5uz, 99);

    CHECK_EQ(sut.segment_size(0uz), 5uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut.data_size(), 7uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3, 99, 99}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers,
    "resize(seg, n, value) should do nothing when n == segment_size"
) {
    sut.resize(0uz, 3uz, 99);

    CHECK_EQ(sut.segment_size(0uz), 3uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_element_modifiers, "resize on last segment should work correctly"
) {
    sut.resize(1uz, 4uz, 88);

    CHECK_EQ(sut.segment_size(0uz), 3uz);
    CHECK_EQ(sut.segment_size(1uz), 4uz);
    CHECK_EQ(sut.data_size(), 7uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 88, 88}));
}

struct test_flat_jagged_vector_complex_operations {
    using sut_type = gl::flat_jagged_vector<int>;
};

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_complex_operations,
    "interleaved segment and element operations should work correctly"
) {
    sut_type sut;

    sut.push_back({1, 2});
    sut.insert(0uz, 1uz, 10);
    sut.push_back(0uz, 20);
    sut.push_back({3, 4, 5});

    CHECK_EQ(sut.size(), 2uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 10, 2, 20}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{3, 4, 5}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_complex_operations, "clearing and refilling should work correctly"
) {
    sut_type sut;
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));

    sut.clear();

    CHECK(sut.empty());

    sut.push_back({9, 8, 7});

    CHECK_EQ(sut.size(), 1uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut.data_view(), std::vector<int>{9, 8, 7}));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_complex_operations,
    "large flat_jagged_vector operations should maintain integrity"
) {
    sut_type sut;

    for (int i = 0; i < 100; ++i) {
        std::vector<int> segment;
        for (int j = 0; j < 10; ++j)
            segment.push_back(i * 10 + j);
        sut.push_back(segment);
    }

    CHECK_EQ(sut.size(), 100uz);
    CHECK_EQ(sut.data_size(), 1000uz);
    for (int i = 0; i < 100; ++i) {
        auto seg = sut[static_cast<std::size_t>(i)];
        for (int j = 0; j < 10; ++j)
            CHECK_EQ(seg[gl::to_diff(j)], i * 10 + j);
    }
}

struct test_flat_jagged_vector_iterators {
    using sut_type = gl::flat_jagged_vector<int>;

    sut_type sut{
        {1, 2, 3},
        {4, 5},
        {6}
    };
    std::vector<std::vector<int>> segments{
        {1, 2, 3},
        {4, 5},
        {6}
    };
};

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "begin() should return iterator to first segment"
) {
    auto it = sut.begin();
    CHECK(std::ranges::equal(*it, segments.front()));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "end() should return iterator past last segment"
) {
    auto it_begin = sut.begin();
    auto it_end = sut.end();
    CHECK_EQ(it_end - it_begin, segments.size());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "const begin() should return const iterator to first segment"
) {
    const auto& const_sut = sut;
    auto it = const_sut.begin();
    CHECK(std::ranges::equal(*it, segments.front()));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "const end() should return const iterator past last segment"
) {
    const auto& const_sut = sut;
    auto it_begin = const_sut.begin();
    auto it_end = const_sut.end();
    CHECK_EQ(it_end - it_begin, segments.size());
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "cbegin() should return const iterator") {
    auto it = sut.cbegin();
    CHECK(std::ranges::equal(*it, segments.front()));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "cend() should return const iterator") {
    auto it_begin = sut.cbegin();
    auto it_end = sut.cend();
    CHECK_EQ(it_end - it_begin, segments.size());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators,
    "non-const iterator should convert to const iterator implicitly"
) {
    auto non_const_it = sut.begin();
    typename sut_type::const_iterator const_it = non_const_it;
    CHECK(std::ranges::equal(*const_it, segments.front()));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "dereferencing iterator should return segment"
) {
    auto it = sut.begin();
    CHECK(std::ranges::equal(*it, segments[0uz]));

    ++it;
    CHECK(std::ranges::equal(*it, segments[1uz]));

    ++it;
    CHECK(std::ranges::equal(*it, segments[2uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "operator[] should access segment at offset") {
    auto it = sut.begin();
    CHECK(std::ranges::equal(it[0uz], segments[0uz]));
    CHECK(std::ranges::equal(it[1uz], segments[1uz]));
    CHECK(std::ranges::equal(it[2uz], segments[2uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "pre-increment should advance iterator") {
    auto it = sut.begin();
    CHECK(std::ranges::equal(*it, segments[0uz]));

    auto& ret = ++it;
    CHECK(std::ranges::equal(*ret, segments[1uz]));
    CHECK(std::ranges::equal(*it, segments[1uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "post-increment should return old iterator") {
    auto it = sut.begin();
    CHECK(std::ranges::equal(*it, segments[0uz]));

    auto old_it = it++;
    CHECK(std::ranges::equal(*old_it, segments[0uz]));
    CHECK(std::ranges::equal(*it, segments[1uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "pre-decrement should move iterator backward") {
    auto it = sut.end();
    --it;
    CHECK(std::ranges::equal(*it, segments[2uz]));

    auto& ret = --it;
    CHECK(std::ranges::equal(*ret, segments[1uz]));
    CHECK(std::ranges::equal(*it, segments[1uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "post-decrement should return old iterator") {
    auto it = sut.end();
    --it;
    CHECK(std::ranges::equal(*it, segments[2uz]));

    auto old_it = it--;
    CHECK(std::ranges::equal(*old_it, segments[2uz]));
    CHECK(std::ranges::equal(*it, segments[1uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "operator+= should advance iterator") {
    auto it = sut.begin();
    it += 2;
    CHECK(std::ranges::equal(*it, segments[2uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "operator-= should move iterator backward") {
    auto it = sut.end();
    it -= 1;
    CHECK(std::ranges::equal(*it, segments[2uz]));

    it -= 2;
    CHECK(std::ranges::equal(*it, segments[0uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "operator+ should create new iterator") {
    auto it = sut.begin();
    auto new_it = it + 1;

    CHECK(std::ranges::equal(*it, segments[0uz]));
    CHECK(std::ranges::equal(*new_it, segments[1uz]));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "reverse operator+ should create new iterator"
) {
    auto it = sut.begin();
    auto new_it = 2 + it;

    CHECK(std::ranges::equal(*it, segments[0uz]));
    CHECK(std::ranges::equal(*new_it, segments[2uz]));
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "operator- should create new iterator") {
    auto it = sut.end();
    auto new_it = it - 1;

    CHECK(std::ranges::equal(*new_it, segments[2uz]));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "operator- with two iterators should give distance"
) {
    auto it1 = sut.begin();
    auto it2 = sut.end();

    CHECK_EQ(it2 - it1, sut.size());
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "operator== should compare iterators") {
    auto it1 = sut.begin();
    auto it2 = sut.begin();
    auto it3 = sut.begin() + 1;

    CHECK_EQ(it1, it2);
    CHECK_NE(it1, it3);
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "operator<=> should compare iterators") {
    auto it1 = sut.begin();
    auto it2 = sut.begin() + 1;
    auto it3 = sut.begin() + 2;

    CHECK_LT(it1, it2);
    CHECK_LT(it2, it3);
    CHECK_LE(it1, it2);
    CHECK_LE(it1, it1);
    CHECK_GT(it2, it1);
    CHECK_GT(it3, it2);
    CHECK_GE(it2, it1);
    CHECK_GE(it2, it2);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "range-based for loop should iterate all segments"
) {
    std::size_t idx = 0uz;
    for (auto seg : sut)
        CHECK(std::ranges::equal(seg, segments[idx++]));
    CHECK_EQ(idx, 3uz);
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "const range-based for loop should iterate all segments"
) {
    const auto& const_sut = sut;
    std::size_t idx = 0uz;
    for (auto seg : const_sut)
        CHECK(std::ranges::equal(seg, segments[idx++]));
    CHECK_EQ(idx, 3uz);
}

TEST_CASE_FIXTURE(test_flat_jagged_vector_iterators, "rbegin() should return reverse iterator") {
    auto it = sut.rbegin();
    CHECK(std::ranges::equal(*it, segments.back()));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "rend() should return reverse iterator past first"
) {
    auto it_rbegin = sut.rbegin();
    auto it_rend = sut.rend();
    CHECK_EQ(it_rend - it_rbegin, segments.size());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "crbegin() should return const reverse iterator"
) {
    auto it = sut.crbegin();
    CHECK(std::ranges::equal(*it, segments.back()));
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "crend() should return const reverse iterator"
) {
    auto it_rbegin = sut.crbegin();
    auto it_rend = sut.crend();
    CHECK_EQ(it_rend - it_rbegin, segments.size());
}

TEST_CASE_FIXTURE(
    test_flat_jagged_vector_iterators, "reverse range-based for loop should iterate in reverse"
) {
    std::size_t idx = 2uz;
    for (auto it : std::ranges::reverse_view(sut)) {
        CHECK(std::ranges::equal(it, segments[idx]));
        if (idx > 0uz)
            --idx;
    }
}

TEST_SUITE_END(); // test_flat_jagged_vector

} // namespace gl_testing
