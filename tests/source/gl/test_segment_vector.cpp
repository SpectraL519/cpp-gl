#include <gl/types/segment_vector.hpp>

#include <doctest.h>

#include <algorithm>
#include <functional>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_segment_vector");

struct test_segment_vector_constructors {
    using sut_type = segment_vector<int>;
};

TEST_CASE_FIXTURE(
    test_segment_vector_constructors, "default constructor should create empty segment_vector"
) {
    sut_type sut;
    CHECK(sut.empty());
    CHECK_EQ(sut.size(), 0uz);
    CHECK_EQ(sut.data_size(), 0uz);
}

TEST_CASE_FIXTURE(test_segment_vector_constructors, "copy constructor should create an equal copy") {
    sut_type original;
    original.push_back({1, 2, 3});
    original.push_back({4, 5});

    sut_type copy = original;

    CHECK_EQ(copy, original);
    CHECK_EQ(copy.size(), 2uz);
    CHECK_EQ(copy.data_size(), 5uz);
}

TEST_CASE_FIXTURE(test_segment_vector_constructors, "copy assignment should create an equal copy") {
    sut_type original;
    original.push_back({1, 2, 3});
    original.push_back({4, 5});

    sut_type target;
    target = original;

    CHECK_EQ(target, original);
    CHECK_EQ(target.size(), 2uz);
    CHECK_EQ(target.data_size(), 5uz);
}

TEST_CASE_FIXTURE(test_segment_vector_constructors, "move constructor should transfer ownership") {
    sut_type source;
    source.push_back({1, 2, 3});
    source.push_back({4, 5});

    sut_type dest = std::move(source);

    CHECK_EQ(dest.size(), 2uz);
    CHECK_EQ(dest.data_size(), 5uz);
    CHECK(source.empty());
}

TEST_CASE_FIXTURE(test_segment_vector_constructors, "move assignment should transfer ownership") {
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
    test_segment_vector_constructors, "move assignment should handle self-assignment correctly"
) {
    sut_type sut;
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    sut = std::move(sut);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.data_size(), 5uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_constructors, "initializer list constructor should initialize segments"
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
    test_segment_vector_constructors, "range constructor should initialize from nested ranges"
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

struct test_segment_vector_comparison {
    using sut_type = segment_vector<int>;
};

TEST_CASE_FIXTURE(
    test_segment_vector_comparison, "equality operator should return true for equal segment_vectors"
) {
    sut_type sv1{
        {1, 2},
        {3, 4}
    };
    sut_type sv2{
        {1, 2},
        {3, 4}
    };

    CHECK_EQ(sv1, sv2);
}

TEST_CASE_FIXTURE(
    test_segment_vector_comparison,
    "inequality operator should return true for different segment_vectors"
) {
    sut_type sv1{
        {1, 2},
        {3, 4}
    };
    sut_type sv2{
        {1, 2},
        {3, 5}
    };
    sut_type sv3{
        {1, 2},
        {3, 4},
        {5}
    };

    CHECK_NE(sv1, sv2);
    CHECK_NE(sv1, sv3);
}

TEST_CASE_FIXTURE(test_segment_vector_comparison, "empty segment_vectors should be equal") {
    sut_type sv1;
    sut_type sv2;

    CHECK_EQ(sv1, sv2);
}

struct test_segment_vector_capacity {
    using sut_type = segment_vector<int>;
    sut_type sut;
};

TEST_CASE_FIXTURE(test_segment_vector_capacity, "size should return the number of segments") {
    CHECK_EQ(sut.size(), 0uz);

    sut.push_back({1, 2, 3});
    CHECK_EQ(sut.size(), 1uz);

    sut.push_back({4, 5});
    CHECK_EQ(sut.size(), 2uz);

    sut.push_back({6});
    CHECK_EQ(sut.size(), 3uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_capacity, "empty should return true only when there are no segments"
) {
    CHECK(sut.empty());

    sut.push_back({1});
    CHECK_FALSE(sut.empty());

    sut.pop_back();
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(
    test_segment_vector_capacity, "reserve_segments should reserve space for segments"
) {
    sut.reserve_segments(10uz);
    CHECK_EQ(sut.segments_capacity(), 10uz);
}

TEST_CASE_FIXTURE(test_segment_vector_capacity, "reserve_data should reserve space for data") {
    sut.reserve_data(100uz);
    CHECK_EQ(sut.data_capacity(), 100uz);
}

TEST_CASE_FIXTURE(test_segment_vector_capacity, "shrink_to_fit should reduce capacity") {
    sut.push_back({1, 2, 3});
    sut.shrink_to_fit();

    CHECK_EQ(sut.segments_capacity(), 1uz);
    CHECK_EQ(sut.data_capacity(), 3uz);
}

TEST_CASE_FIXTURE(test_segment_vector_capacity, "clear should remove all segments and data") {
    sut.push_back({1, 2, 3});
    sut.push_back({4, 5});

    sut.clear();

    CHECK(sut.empty());
    CHECK_EQ(sut.data_size(), 0uz);
}

struct test_segment_vector_segment_accessors {
    using sut_type = segment_vector<int>;

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
};

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors, "operator[] should return segment at given index"
) {
    auto seg0 = sut[0uz];
    CHECK(std::ranges::equal(seg0, data[0uz]));

    auto seg1 = sut[1uz];
    CHECK(std::ranges::equal(seg1, data[1uz]));

    auto seg2 = sut[2uz];
    CHECK(std::ranges::equal(seg2, data[2uz]));
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors,
    "const operator[] should return const segment at given index"
) {
    const auto& const_sut = sut;

    auto seg0 = const_sut[0uz];
    CHECK(std::ranges::equal(seg0, data[0uz]));

    auto seg1 = const_sut[1uz];
    CHECK(std::ranges::equal(seg1, data[1uz]));
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors, "at() should return segment at given index"
) {
    auto seg0 = sut.at(0uz);
    CHECK(std::ranges::equal(seg0, data[0uz]));

    auto seg1 = sut.at(1uz);
    CHECK(std::ranges::equal(seg1, data[1uz]));

    auto seg2 = sut.at(2uz);
    CHECK(std::ranges::equal(seg2, data[2uz]));
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors, "at() should throw for out of range index"
) {
    CHECK_THROWS_AS(static_cast<void>(sut.at(3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors, "const at() should return const segment at given index"
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
    test_segment_vector_segment_accessors, "const at() should throw for out of range index"
) {
    const auto& const_sut = sut;
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors, "segments() should return a view of all segments"
) {
    auto n_segments = 0uz;
    for (auto seg : sut.segments()) {
        CHECK(std::ranges::equal(seg, data[n_segments]));
        n_segments++;
    }

    CHECK_EQ(n_segments, 3uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors,
    "const segments() should return a const view of all segments"
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
    test_segment_vector_segment_accessors, "segment_size() should return the size of a segment"
) {
    CHECK_EQ(sut.segment_size(0uz), 3uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut.segment_size(2uz), 1uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors, "data_size() should return the total number of elements"
) {
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(test_segment_vector_segment_accessors, "data() should return a span of all data") {
    auto data = sut.data();
    CHECK(std::ranges::equal(data, flat_data));
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_accessors, "const data() should return a const span of all data"
) {
    const auto& const_sut = sut;
    auto data = const_sut.data();
    CHECK(std::ranges::equal(data, flat_data));
}

struct test_segment_vector_element_accessors {
    using sut_type = segment_vector<int>;
    sut_type sut{
        {1, 2, 3},
        {4, 5},
        {6}
    };
};

TEST_CASE_FIXTURE(
    test_segment_vector_element_accessors,
    "operator[](seg, pos) should return element at given segment and position"
) {
    CHECK_EQ(sut[0uz, 0uz], 1);
    CHECK_EQ(sut[0uz, 1uz], 2);
    CHECK_EQ(sut[0uz, 2uz], 3);
    CHECK_EQ(sut[1uz, 0uz], 4);
    CHECK_EQ(sut[1uz, 1uz], 5);
    CHECK_EQ(sut[2uz, 0uz], 6);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_accessors, "const operator[](seg, pos) should return const element"
) {
    const auto& const_sut = sut;

    CHECK_EQ(const_sut[0uz, 0uz], 1);
    CHECK_EQ(const_sut[0uz, 1uz], 2);
    CHECK_EQ(const_sut[0uz, 2uz], 3);
    CHECK_EQ(const_sut[1uz, 0uz], 4);
    CHECK_EQ(const_sut[1uz, 1uz], 5);
    CHECK_EQ(const_sut[2uz, 0uz], 6);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_accessors,
    "at(seg, pos) should return element at given segment and position"
) {
    CHECK_EQ(sut.at(0uz, 0uz), 1);
    CHECK_EQ(sut.at(0uz, 1uz), 2);
    CHECK_EQ(sut.at(0uz, 2uz), 3);
    CHECK_EQ(sut.at(1uz, 0uz), 4);
    CHECK_EQ(sut.at(1uz, 1uz), 5);
    CHECK_EQ(sut.at(2uz, 0uz), 6);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_accessors, "at(seg, pos) should throw for invalid segment"
) {
    CHECK_THROWS_AS(static_cast<void>(sut.at(3uz, 0uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_accessors, "at(seg, pos) should throw for invalid position"
) {
    CHECK_THROWS_AS(static_cast<void>(sut.at(0uz, 3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(1uz, 2uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(2uz, 1uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_accessors, "const at(seg, pos) should return const element"
) {
    const auto& const_sut = sut;

    CHECK_EQ(const_sut.at(0uz, 0uz), 1);
    CHECK_EQ(const_sut.at(0uz, 1uz), 2);
    CHECK_EQ(const_sut.at(0uz, 2uz), 3);
    CHECK_EQ(const_sut.at(1uz, 0uz), 4);
    CHECK_EQ(const_sut.at(1uz, 1uz), 5);
    CHECK_EQ(const_sut.at(2uz, 0uz), 6);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_accessors, "const at(seg, pos) should throw for invalid segment"
) {
    const auto& const_sut = sut;

    CHECK_THROWS_AS(static_cast<void>(const_sut.at(3uz, 0uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_accessors, "const at(seg, pos) should throw for invalid position"
) {
    const auto& const_sut = sut;

    CHECK_THROWS_AS(static_cast<void>(const_sut.at(0uz, 3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(1uz, 2uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(2uz, 1uz)), std::out_of_range);
}

struct test_segment_vector_segment_modifiers {
    using sut_type = segment_vector<int>;

    sut_type sut;
    std::vector<int> seg0{1, 2, 3};
    std::vector<int> seg1{4, 5};
    std::vector<int> seg2{6, 7, 8};
};

TEST_CASE_FIXTURE(
    test_segment_vector_segment_modifiers, "push_back with span should add new segment"
) {
    sut.push_back(std::span<const int>{seg0});

    CHECK_EQ(sut.size(), 1uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut[0uz], seg0));
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_modifiers, "push_back with vector should add new segment"
) {
    sut.push_back(seg0);

    CHECK_EQ(sut.size(), 1uz);
    CHECK(std::ranges::equal(sut[0uz], seg0));
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_modifiers, "push_back with initializer list should add new segment"
) {
    sut.push_back({1, 2, 3});

    CHECK_EQ(sut.size(), 1uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_modifiers, "multiple push_back calls should add multiple segments"
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

TEST_CASE_FIXTURE(test_segment_vector_segment_modifiers, "pop_back should remove last segment") {
    sut.push_back(seg0);
    sut.push_back(seg1);

    sut.pop_back();

    CHECK_EQ(sut.size(), 1uz);
    CHECK_EQ(sut.data_size(), seg0.size());
    CHECK(std::ranges::equal(sut[0uz], seg0));
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_modifiers, "pop_back on empty container should do nothing"
) {
    sut.pop_back();
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(
    test_segment_vector_segment_modifiers, "pop_back should remove all segments sequentially"
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
    test_segment_vector_segment_modifiers, "insert should add segment at given position"
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
    test_segment_vector_segment_modifiers, "erase should remove segment at given position"
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

TEST_CASE_FIXTURE(test_segment_vector_segment_modifiers, "erase should update offsets correctly") {
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

struct test_segment_vector_element_modifiers {
    using sut_type = segment_vector<int>;
    sut_type sut{
        {1, 2, 3},
        {4, 5}
    };
};

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers, "push_back element should add element to segment"
) {
    sut.push_back(0uz, 10);

    CHECK_EQ(sut[0uz].back(), 10);
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers, "emplace_back should add element to segment"
) {
    sut.emplace_back(0uz, 10);

    CHECK_EQ(sut[0uz].back(), 10);
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers,
    "pop_back element should remove last element from segment"
) {
    sut.pop_back(0uz);

    CHECK_EQ(sut.segment_size(0uz), 2uz);
    CHECK_EQ(sut.data_size(), 4uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers, "pop_back on empty segment should do nothing"
) {
    sut_type empty_sv;
    empty_sv.push_back({});
    empty_sv.pop_back(0uz);

    CHECK_EQ(empty_sv.segment_size(0uz), 0uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers,
    "insert element should add element at position in segment"
) {
    sut.insert(0uz, 1uz, 10);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 10, 2, 3}));
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers, "emplace should add element at position in segment"
) {
    sut.emplace(0uz, 1uz, 10);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 10, 2, 3}));
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers,
    "erase element should remove element at position in segment"
) {
    sut.erase(0uz, 1uz);

    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
    CHECK_EQ(sut.data_size(), 4uz);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers, "insert element should update offsets for other segments"
) {
    sut.insert(0uz, 0uz, 0);

    CHECK_EQ(sut.segment_size(0uz), 4uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut[1uz].front(), 4);
}

TEST_CASE_FIXTURE(
    test_segment_vector_element_modifiers, "erase element should update offsets for other segments"
) {
    sut.erase(0uz, 0uz);

    CHECK_EQ(sut.segment_size(0uz), 2uz);
    CHECK_EQ(sut.segment_size(1uz), 2uz);
    CHECK_EQ(sut[0uz].front(), 2);
    CHECK_EQ(sut[1uz].front(), 4);
}

struct test_segment_vector_complex_operations {
    using sut_type = segment_vector<int>;
};

TEST_CASE_FIXTURE(
    test_segment_vector_complex_operations,
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
    test_segment_vector_complex_operations, "clearing and refilling should work correctly"
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
    CHECK(std::ranges::equal(sut.data(), std::vector<int>{9, 8, 7}));
}

TEST_CASE_FIXTURE(
    test_segment_vector_complex_operations,
    "large segment_vector operations should maintain integrity"
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
        auto seg = sut[i];
        for (int j = 0; j < 10; ++j)
            CHECK_EQ(seg[j], i * 10 + j);
    }
}

// TEST_SUITE_END();

} // namespace gl_testing
