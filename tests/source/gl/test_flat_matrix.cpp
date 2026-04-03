#include "doctest.h"
#include "testing/common/wrnsup.hpp"

#include <gl/types/flat_matrix.hpp>

#include <algorithm>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_flat_matrix");

struct test_flat_matrix_constructors {
    using sut_type = gl::flat_matrix<int>;
};

TEST_CASE_FIXTURE(
    test_flat_matrix_constructors, "default constructor should create empty flat_matrix"
) {
    sut_type sut;
    CHECK(sut.empty());
    CHECK_EQ(sut.size(), 0uz);
    CHECK_EQ(sut.n_rows(), 0uz);
    CHECK_EQ(sut.n_cols(), 0uz);
    CHECK_EQ(sut.data_size(), 0uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_constructors, "copy constructor should create an equal copy") {
    sut_type original;
    original.push_row({1, 2, 3});
    original.push_row({4, 5, 6});

    sut_type copy = original;

    CHECK_EQ(copy, original);
    CHECK_EQ(copy.size(), 2uz);
    CHECK_EQ(copy.n_rows(), 2uz);
    CHECK_EQ(copy.n_cols(), 3uz);
    CHECK_EQ(copy.data_size(), 6uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_constructors, "copy assignment should create an equal copy") {
    sut_type original;
    original.push_row({1, 2, 3});
    original.push_row({4, 5, 6});

    sut_type dest;
    dest = original;

    CHECK_EQ(dest, original);
    CHECK_EQ(dest.size(), 2uz);
    CHECK_EQ(dest.n_rows(), 2uz);
    CHECK_EQ(dest.n_cols(), 3uz);
    CHECK_EQ(dest.data_size(), 6uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_constructors, "move constructor should transfer ownership") {
    sut_type source;
    source.push_row({1, 2, 3});
    source.push_row({4, 5, 6});

    sut_type dest = std::move(source);

    CHECK_EQ(dest.size(), 2uz);
    CHECK_EQ(dest.n_rows(), 2uz);
    CHECK_EQ(dest.n_cols(), 3uz);
    CHECK_EQ(dest.data_size(), 6uz);
    CHECK(source.empty());
    CHECK_EQ(source.n_rows(), 0uz);
    CHECK_EQ(source.n_cols(), 0uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_constructors, "move assignment should transfer ownership") {
    sut_type source;
    source.push_row({1, 2, 3});
    source.push_row({4, 5, 6});

    sut_type dest;
    dest = std::move(source);

    CHECK_EQ(dest.size(), 2uz);
    CHECK_EQ(dest.n_rows(), 2uz);
    CHECK_EQ(dest.n_cols(), 3uz);
    CHECK_EQ(dest.data_size(), 6uz);
    CHECK(source.empty());
    CHECK_EQ(source.n_rows(), 0uz);
    CHECK_EQ(source.n_cols(), 0uz);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_constructors, "move assignment should handle self-assignment correctly"
) {
    sut_type sut;
    sut.push_row({1, 2, 3});
    sut.push_row({4, 5, 6});

    SUPPRESS_WARNING_BEGIN("-Wself-move");
    sut = std::move(sut);
    SUPPRESS_WARNING_END;

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_constructors,
    "(n_rows, n_cols) constructor should build a matrixed filled with a default type value"
) {
    sut_type sut(2uz, 3uz);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::all_of(sut.data_view(), [](const auto v) { return v == int{}; }));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_constructors,
    "(n_rows, n_cols, value) constructor should build a matrixed filled with the given value"
) {
    int fill_value = 123;
    sut_type sut(2uz, 3uz, fill_value);

    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::all_of(sut.data_view(), [fill_value](const auto v) {
        return v == fill_value;
    }));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_constructors,
    "initializer list constructor should initialize matrix for same-size rows"
) {
    sut_type sut{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 9uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8, 9}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_constructors,
    "initializer list constructor should throw for different size rows"
) {
    const auto create_sut = []() {
        sut_type sut{
            {1, 2, 3},
            {4, 5, 6},
            {7}
        };
    };
    CHECK_THROWS_AS(create_sut(), std::invalid_argument);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_constructors, "range constructor should initialize matrix for same-size rows"
) {
    std::vector<std::vector<int>> data{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    sut_type sut{std::move(data)};

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 9uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8, 9}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_constructors, "range constructor should throw for different size rows"
) {
    std::vector<std::vector<int>> data{
        {1, 2, 3},
        {4, 5, 6},
        {7}
    };
    const auto create_sut = [&data]() { sut_type sut{std::move(data)}; };
    CHECK_THROWS_AS(create_sut(), std::invalid_argument);
}

struct test_flat_matrix_comparison {
    using sut_type = gl::flat_matrix<int>;
};

TEST_CASE_FIXTURE(
    test_flat_matrix_comparison, "equality operator should return true for equal matrices"
) {
    sut_type m1{
        {1, 2},
        {3, 4}
    };
    sut_type m2{
        {1, 2},
        {3, 4}
    };

    CHECK_EQ(m1, m2);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_comparison, "inequality operator should return true for different matrices"
) {
    sut_type m1{
        {1, 2},
        {3, 4}
    };
    sut_type m2{
        {1, 2},
        {3, 5}
    };
    sut_type m3{
        {1, 2},
        {3, 4},
        {5, 6}
    };

    CHECK_NE(m1, m2);
    CHECK_NE(m1, m3);
}

TEST_CASE_FIXTURE(test_flat_matrix_comparison, "empty matrices should be equal") {
    sut_type m1;
    sut_type m2;

    CHECK_EQ(m1, m2);
}

struct test_flat_matrix_capacity {
    using sut_type = gl::flat_matrix<int>;
    sut_type sut;
};

TEST_CASE_FIXTURE(
    test_flat_matrix_capacity,
    "size and n_rows should return the number of rows and n_cols should return the number of "
    "columns"
) {
    CHECK_EQ(sut.size(), 0uz);
    CHECK_EQ(sut.n_rows(), 0uz);
    CHECK_EQ(sut.n_cols(), 0uz);

    sut.push_row({1, 2, 3});
    CHECK_EQ(sut.size(), 1uz);
    CHECK_EQ(sut.n_rows(), 1uz);
    CHECK_EQ(sut.n_cols(), 3uz);

    sut.push_row({4, 5, 6});
    CHECK_EQ(sut.size(), 2uz);
    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);

    sut.push_row({7, 8, 9});
    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_capacity,
    "empty should return true only when there are no elements in the matrix"
) {
    CHECK(sut.empty());

    sut.push_row({1, 2, 3});
    CHECK_FALSE(sut.empty());

    sut.pop_row();
    CHECK(sut.empty());

    sut.push_col({1, 2, 3});
    CHECK_FALSE(sut.empty());

    sut.pop_col();
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(test_flat_matrix_capacity, "reserve_data should reserve space for elements") {
    sut.reserve_data(10uz);
    CHECK_EQ(sut.data_capacity(), 10uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_capacity, "shrink_to_fit should reduce capacity") {
    sut.push_row({1, 2, 3});
    sut.shrink_to_fit();

    CHECK_EQ(sut.data_capacity(), 3uz);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_capacity,
    "resize(n_rows, n_cols, v) should shrink container when n_rows < current n_rows"
) {
    sut.push_row({1, 2, 3});
    sut.push_row({4, 5, 6});
    sut.push_row({7, 8, 9});

    REQUIRE_EQ(sut.n_rows(), 3uz);
    REQUIRE_EQ(sut.n_cols(), 3uz);
    REQUIRE_EQ(sut.data_size(), 9uz);

    sut.resize(2uz, 3uz, -1);

    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_capacity,
    "resize(n_rows, n_cols, v) should shrink container when c_cols < current n_cols"
) {
    sut.push_row({1, 2, 3});
    sut.push_row({4, 5, 6});
    sut.push_row({7, 8, 9});

    REQUIRE_EQ(sut.n_rows(), 3uz);
    REQUIRE_EQ(sut.n_cols(), 3uz);
    REQUIRE_EQ(sut.data_size(), 9uz);

    sut.resize(3uz, 2uz, -1);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 2uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_capacity,
    "resize(n_rows, n_cols, v) should grow container with the fiven value when n_rows > current "
    "n_rows"
) {
    sut.push_row({1, 2, 3});
    sut.push_row({4, 5, 6});
    sut.push_row({7, 8, 9});

    REQUIRE_EQ(sut.n_rows(), 3uz);
    REQUIRE_EQ(sut.n_cols(), 3uz);
    REQUIRE_EQ(sut.data_size(), 9uz);

    sut.resize(4uz, 3uz, -1);

    CHECK_EQ(sut.n_rows(), 4uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 12uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8, 9}));
    CHECK(std::ranges::equal(sut[3uz], std::vector<int>{-1, -1, -1}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_capacity,
    "resize(n_rows, n_cols, v) should grow container with default type value when n_cols > current "
    "n_cols"
) {
    sut.push_row({1, 2, 3});
    sut.push_row({4, 5, 6});
    sut.push_row({7, 8, 9});

    REQUIRE_EQ(sut.n_rows(), 3uz);
    REQUIRE_EQ(sut.n_cols(), 3uz);
    REQUIRE_EQ(sut.data_size(), 9uz);

    sut.resize(3uz, 4uz, -1);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 4uz);
    CHECK_EQ(sut.data_size(), 12uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3, -1}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6, -1}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8, 9, -1}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_capacity,
    "resize(n_rows, n_cols, v) should properly change the dimensions of the matrix for mixed size "
    "differences"
) {
    sut.push_row({1, 2, 3});
    sut.push_row({4, 5, 6});
    sut.push_row({7, 8, 9});

    REQUIRE_EQ(sut.n_rows(), 3uz);
    REQUIRE_EQ(sut.n_cols(), 3uz);
    REQUIRE_EQ(sut.data_size(), 9uz);

    sut.resize(4uz, 4uz, -1);

    CHECK_EQ(sut.n_rows(), 4uz);
    CHECK_EQ(sut.n_cols(), 4uz);
    CHECK_EQ(sut.data_size(), 16uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3, -1}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6, -1}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8, 9, -1}));
    CHECK(std::ranges::equal(sut[3uz], std::vector<int>{-1, -1, -1, -1}));

    sut.resize(3uz, 5uz, -2);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 5uz);
    CHECK_EQ(sut.data_size(), 15uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3, -1, -2}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6, -1, -2}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8, 9, -1, -2}));

    sut.resize(5uz, 3uz, -3);

    CHECK_EQ(sut.n_rows(), 5uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 15uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8, 9}));
    CHECK(std::ranges::equal(sut[3uz], std::vector<int>{-3, -3, -3}));
    CHECK(std::ranges::equal(sut[4uz], std::vector<int>{-3, -3, -3}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_capacity,
    "resize(n_rows, n_cols, v) should do nothing when dimensions are not changed"
) {
    sut.push_row({1, 2, 3});
    sut.push_row({4, 5, 6});
    sut.push_row({7, 8, 9});

    REQUIRE_EQ(sut.n_rows(), 3uz);
    REQUIRE_EQ(sut.n_cols(), 3uz);
    REQUIRE_EQ(sut.data_size(), 9uz);

    sut.resize(3uz, 3uz, -1);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 9uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{1, 2, 3}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 5, 6}));
    CHECK(std::ranges::equal(sut[2uz], std::vector<int>{7, 8, 9}));
}

TEST_CASE_FIXTURE(test_flat_matrix_capacity, "clear should remove all data") {
    sut.push_row({1, 2, 3});
    sut.push_row({4, 5, 6});

    sut.clear();

    CHECK(sut.empty());
    CHECK_EQ(sut.n_rows(), 0uz);
    CHECK_EQ(sut.n_cols(), 0uz);
    CHECK_EQ(sut.data_size(), 0uz);
}

struct test_flat_matrix_accessors {
    using sut_type = gl::flat_matrix<int>;

    sut_type sut{
        {1, 2, 3},
        {4, 5, 6}
    };

    std::vector<int> sut_row0{1, 2, 3};
    std::vector<int> sut_row1{4, 5, 6};

    std::vector<int> sut_col0{1, 4};
    std::vector<int> sut_col1{2, 5};
    std::vector<int> sut_col2{3, 6};

    std::vector<std::vector<int>> sut_rows{sut_row0, sut_row1}; // rename to sut_rows
    std::vector<std::vector<int>> sut_cols{sut_col0, sut_col1, sut_col2};
    std::vector<int> flat_data{1, 2, 3, 4, 5, 6};

    std::size_t sut_n_rows = 2uz;
    std::size_t sut_n_cols = 3uz;

    int dummy_value = 111;
};

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "operator[r] should return row at given index") {
    auto r0 = sut[0uz];
    CHECK(std::ranges::equal(r0, sut_rows[0uz]));
    r0.front() = dummy_value;
    CHECK_EQ(sut[0uz].front(), dummy_value);

    auto r1 = sut[1uz];
    CHECK(std::ranges::equal(r1, sut_rows[1uz]));
    r1.front() = dummy_value;
    CHECK_EQ(sut[1uz].front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const operator[r] should return const row at given index"
) {
    const auto& const_sut = sut;

    auto r0 = const_sut[0uz];
    CHECK(std::ranges::equal(r0, sut_rows[0uz]));

    auto r1 = const_sut[1uz];
    CHECK(std::ranges::equal(r1, sut_rows[1uz]));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "operator[](r, r) should return element at given row and position"
) {
    CHECK_EQ(sut[0uz, 0uz], sut_row0[0uz]);
    sut[0uz, 0uz] = dummy_value;
    CHECK_EQ(sut[0uz, 0uz], dummy_value);

    CHECK_EQ(sut[0uz, 1uz], sut_row0[1uz]);
    sut[0uz, 1uz] = dummy_value;
    CHECK_EQ(sut[0uz, 1uz], dummy_value);

    CHECK_EQ(sut[0uz, 2uz], sut_row0[2uz]);
    sut[0uz, 2uz] = dummy_value;
    CHECK_EQ(sut[0uz, 2uz], dummy_value);

    CHECK_EQ(sut[1uz, 0uz], sut_row1[0uz]);
    sut[1uz, 0uz] = dummy_value;
    CHECK_EQ(sut[1uz, 0uz], dummy_value);

    CHECK_EQ(sut[1uz, 1uz], sut_row1[1uz]);
    sut[1uz, 1uz] = dummy_value;
    CHECK_EQ(sut[1uz, 1uz], dummy_value);

    CHECK_EQ(sut[1uz, 2uz], sut_row1[2uz]);
    sut[1uz, 2uz] = dummy_value;
    CHECK_EQ(sut[1uz, 2uz], dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const operator[](r, c) should return const element") {
    const auto& const_sut = sut;

    CHECK_EQ(const_sut[0uz, 0uz], sut_row0[0uz]);
    CHECK_EQ(const_sut[0uz, 1uz], sut_row0[1uz]);
    CHECK_EQ(const_sut[0uz, 2uz], sut_row0[2uz]);
    CHECK_EQ(const_sut[1uz, 0uz], sut_row1[0uz]);
    CHECK_EQ(const_sut[1uz, 1uz], sut_row1[1uz]);
    CHECK_EQ(const_sut[1uz, 2uz], sut_row1[2uz]);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "at(r) should return row at given index") {
    auto r0 = sut.at(0uz);
    CHECK(std::ranges::equal(r0, sut_rows[0uz]));
    r0.front() = dummy_value;
    CHECK_EQ(sut.at(0uz).front(), dummy_value);

    auto r1 = sut.at(1uz);
    CHECK(std::ranges::equal(r1, sut_rows[1uz]));
    r1.front() = dummy_value;
    CHECK_EQ(sut.at(1uz).front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "at(r) should throw for out of range index") {
    CHECK_THROWS_AS(static_cast<void>(sut.at(2uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const at(r) should return const row at given index") {
    const auto& const_sut = sut;

    auto r0 = const_sut.at(0uz);
    CHECK(std::ranges::equal(r0, sut_rows[0uz]));

    auto r1 = const_sut.at(1uz);
    CHECK(std::ranges::equal(r1, sut_rows[1uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const at(r) should throw for out of range index") {
    const auto& const_sut = sut;
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(2uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "at(r, r) should return element at given row and position"
) {
    CHECK_EQ(sut.at(0uz, 0uz), sut_row0[0uz]);
    sut.at(0uz, 0uz) = dummy_value;
    CHECK_EQ(sut.at(0uz, 0uz), dummy_value);

    CHECK_EQ(sut.at(0uz, 1uz), sut_row0[1uz]);
    sut.at(0uz, 1uz) = dummy_value;
    CHECK_EQ(sut.at(0uz, 1uz), dummy_value);

    CHECK_EQ(sut.at(0uz, 2uz), sut_row0[2uz]);
    sut.at(0uz, 2uz) = dummy_value;
    CHECK_EQ(sut.at(0uz, 2uz), dummy_value);

    CHECK_EQ(sut.at(1uz, 0uz), sut_row1[0uz]);
    sut.at(1uz, 0uz) = dummy_value;
    CHECK_EQ(sut.at(1uz, 0uz), dummy_value);

    CHECK_EQ(sut.at(1uz, 1uz), sut_row1[1uz]);
    sut.at(1uz, 1uz) = dummy_value;
    CHECK_EQ(sut.at(1uz, 1uz), dummy_value);

    CHECK_EQ(sut.at(1uz, 2uz), sut_row1[2uz]);
    sut.at(1uz, 2uz) = dummy_value;
    CHECK_EQ(sut.at(1uz, 2uz), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "at(r, c) should throw for invalid row index") {
    CHECK_THROWS_AS(static_cast<void>(sut.at(2uz, 0uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(10uz, 0uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "at(r, c) should throw for invalid col index") {
    CHECK_THROWS_AS(static_cast<void>(sut.at(0uz, 3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.at(1uz, 10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const at(r, c) should return const element") {
    const auto& const_sut = sut;

    CHECK_EQ(const_sut.at(0uz, 0uz), sut_row0[0uz]);
    CHECK_EQ(const_sut.at(0uz, 1uz), sut_row0[1uz]);
    CHECK_EQ(const_sut.at(0uz, 2uz), sut_row0[2uz]);
    CHECK_EQ(const_sut.at(1uz, 0uz), sut_row1[0uz]);
    CHECK_EQ(const_sut.at(1uz, 1uz), sut_row1[1uz]);
    CHECK_EQ(const_sut.at(1uz, 2uz), sut_row1[2uz]);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const at(r, c) should throw for invalid row index") {
    const auto& const_sut = sut;
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(2uz, 0uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(10uz, 0uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const at(r, c) should throw for invalid col index") {
    const auto& const_sut = sut;
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(0uz, 3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.at(1uz, 10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "front() should return the first row") {
    auto front_row = sut.front();
    CHECK(std::ranges::equal(front_row, sut_rows.front()));

    front_row.front() = dummy_value;
    CHECK_EQ(sut.front().front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const front() should return const first row") {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.front(), sut_rows.front()));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "back() should return the last row") {
    auto back_row = sut.back();
    CHECK(std::ranges::equal(back_row, sut_rows.back()));

    back_row.front() = dummy_value;
    CHECK_EQ(sut.back().front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const back() should return const last row") {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.back(), sut_rows.back()));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "front_row() should return the first row") {
    auto front_row = sut.front_row();
    CHECK(std::ranges::equal(front_row, sut_rows.front()));

    front_row.front() = dummy_value;
    CHECK_EQ(sut.front_row().front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const front_row() should return const first row") {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.front_row(), sut_rows.front()));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "back_row() should return the last row") {
    auto back_row = sut.back_row();
    CHECK(std::ranges::equal(back_row, sut_rows.back()));

    back_row.front() = dummy_value;
    CHECK_EQ(sut.back_row().front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const back_row() should return const last row") {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.back_row(), sut_rows.back()));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "front_col() should return the first column") {
    auto front_col = sut.front_col();
    CHECK(std::ranges::equal(front_col, sut_cols.front()));

    front_col.front() = dummy_value;
    CHECK_EQ(sut.front_col().front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const front_col() should return const first column") {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.front_col(), sut_cols.front()));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "back_col() should return the last column") {
    auto back_col = sut.back_col();
    CHECK(std::ranges::equal(back_col, sut_cols.back()));

    back_col.front() = dummy_value;
    CHECK_EQ(sut.back_col().front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const back_col() should return const last column") {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.back_col(), sut_cols.back()));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "row(r) should return row at given index") {
    auto r0 = sut.row(0uz);
    CHECK(std::ranges::equal(r0, sut_rows[0uz]));
    r0.front() = dummy_value;
    CHECK_EQ(sut.row(0uz).front(), dummy_value);

    auto r1 = sut.row(1uz);
    CHECK(std::ranges::equal(r1, sut_rows[1uz]));
    r1.front() = dummy_value;
    CHECK_EQ(sut.row(1uz).front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "row(r) should throw for out of range index") {
    CHECK_THROWS_AS(static_cast<void>(sut.row(2uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.row(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const row(r) should return const row at given index"
) {
    const auto& const_sut = sut;

    auto r0 = const_sut.row(0uz);
    CHECK(std::ranges::equal(r0, sut_rows[0uz]));

    auto r1 = const_sut.row(1uz);
    CHECK(std::ranges::equal(r1, sut_rows[1uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const row(r) should throw for out of range index") {
    const auto& const_sut = sut;
    CHECK_THROWS_AS(static_cast<void>(const_sut.row(2uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.row(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "col(c) should return column at given index") {
    auto c0 = sut.col(0uz);
    CHECK(std::ranges::equal(c0, sut_col0));
    c0.front() = dummy_value;
    CHECK_EQ(sut.col(0uz).front(), dummy_value);

    auto c1 = sut.col(1uz);
    CHECK(std::ranges::equal(c1, sut_col1));
    c1.front() = dummy_value;
    CHECK_EQ(sut.col(1uz).front(), dummy_value);

    auto c2 = sut.col(2uz);
    CHECK(std::ranges::equal(c2, sut_col2));
    c2.front() = dummy_value;
    CHECK_EQ(sut.col(2uz).front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "col(c) should throw for out of range index") {
    CHECK_THROWS_AS(static_cast<void>(sut.col(3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(sut.col(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const col(c) should return const column at given index"
) {
    const auto& const_sut = sut;

    auto c0 = const_sut.col(0uz);
    CHECK(std::ranges::equal(c0, sut_col0));

    auto c1 = const_sut.col(1uz);
    CHECK(std::ranges::equal(c1, sut_col1));

    auto c2 = const_sut.col(2uz);
    CHECK(std::ranges::equal(c2, sut_col2));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const col(c) should throw for out of range index") {
    const auto& const_sut = sut;
    CHECK_THROWS_AS(static_cast<void>(const_sut.col(3uz)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(const_sut.col(10uz)), std::out_of_range);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "rows() should return a view of all rows") {
    auto n_rows = 0uz;
    for (auto row : sut.rows()) {
        CHECK(std::ranges::equal(row, sut_rows[n_rows]));
        const auto orig_val = std::exchange(row.front(), dummy_value);
        CHECK_EQ(sut[n_rows].front(), dummy_value);
        row.front() = orig_val; // revert change

        n_rows++;
    }

    CHECK_EQ(n_rows, sut_n_rows);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const rows() should return a const view of all rows"
) {
    const auto& const_sut = sut;

    auto n_rows = 0uz;
    for (auto row : const_sut.rows()) {
        CHECK(std::ranges::equal(row, sut_rows[n_rows]));
        n_rows++;
    }

    CHECK_EQ(n_rows, sut_n_rows);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "cols() should return a view of all columns") {
    auto n_cols = 0uz;
    for (auto col : sut.cols()) {
        CHECK(std::ranges::equal(col, sut_cols[n_cols]));
        const auto orig_val = std::exchange(col.front(), dummy_value);
        CHECK_EQ(sut.col(n_cols).front(), dummy_value);
        col.front() = orig_val; // revert change

        n_cols++;
    }

    CHECK_EQ(n_cols, sut_n_cols);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const cols() should return a const view of all columns"
) {
    const auto& const_sut = sut;

    auto n_cols = 0uz;
    for (auto col : const_sut.cols()) {
        CHECK(std::ranges::equal(col, sut_cols[n_cols]));
        n_cols++;
    }

    CHECK_EQ(n_cols, sut_n_cols);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors,
    "data_size() should return the total number of elements in the matrix"
) {
    CHECK_EQ(sut.data_size(), 6uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "data_view() should return a span of all data") {
    CHECK(std::ranges::equal(sut.data_view(), flat_data));
    sut.data_view().front() = dummy_value;
    CHECK_EQ(sut.data_view().front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const data_view() should return a const span of all data"
) {
    const auto& const_sut = sut;
    CHECK(std::ranges::equal(const_sut.data_view(), flat_data));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors,
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
    test_flat_matrix_accessors,
    "const data_storage() should return a const reference to the internal vector"
) {
    const auto& const_sut = sut;
    const auto& storage_ref = const_sut.data_storage();

    CHECK(std::ranges::equal(storage_ref, flat_data));
    CHECK_EQ(storage_ref.data(), const_sut.data_view().data());
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors,
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
    test_flat_matrix_accessors,
    "const data_ptr() should return a const raw pointer to the first element"
) {
    const auto& const_sut = sut;
    const auto* ptr = const_sut.data_ptr();

    CHECK_EQ(ptr, const_sut.data_view().data());
    CHECK(std::equal(ptr, ptr + const_sut.data_size(), flat_data.begin()));
}

struct test_flat_matrix_row_modifiers {
    using sut_type = gl::flat_matrix<int>;

    sut_type sut;
    std::vector<int> sut_row0{1, 2, 3};
    std::vector<int> sut_row1{4, 5, 6};
    std::vector<int> sut_row2{7, 8, 9};
};

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "push_row with span should add new row") {
    sut.push_row(std::span<const int>{sut_row0});

    CHECK_EQ(sut.n_rows(), 1uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut[0uz], sut_row0));
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "push_row with vector should add new row") {
    sut.push_row(sut_row0);

    CHECK_EQ(sut.n_rows(), 1uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut[0uz], sut_row0));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "push_row with initializer list should add new row"
) {
    sut.push_row({1, 2, 3});

    CHECK_EQ(sut.n_rows(), 1uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut[0uz], sut_row0));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "push_row should throw for a not matching row size"
) {
    sut.push_row(sut_row0);
    sut.push_row(sut_row1);

    CHECK_THROWS_AS(sut.push_row({11, 22}), std::invalid_argument);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "multiple push_row calls should add multiple rows"
) {
    sut.push_row(sut_row0);
    sut.push_row(sut_row1);
    sut.push_row(sut_row2);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size() + sut_row1.size() + sut_row2.size());
    CHECK(std::ranges::equal(sut[0uz], sut_row0));
    CHECK(std::ranges::equal(sut[1uz], sut_row1));
    CHECK(std::ranges::equal(sut[2uz], sut_row2));
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "push_row with value should add new row") {
    sut.push_row({1, 2, 3});
    sut.push_row(4);

    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 4, 4}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "push_row with value should do nothing if the matrix is empty"
) {
    REQUIRE(sut.empty());

    sut.push_row(0);
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "pop_row should remove last row") {
    sut.push_row(sut_row0);
    sut.push_row(sut_row1);

    REQUIRE(std::ranges::equal(sut.back(), sut_row1));

    sut.pop_row();

    CHECK_EQ(sut.n_rows(), 1uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size());
    CHECK(std::ranges::equal(sut.back(), sut_row0));
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "pop_row on empty container should do nothing") {
    sut.pop_row();
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "pop_row should remove all rows sequentially") {
    sut.push_row(sut_row0);
    sut.push_row(sut_row1);
    sut.push_row(sut_row2);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size() + sut_row1.size() + sut_row2.size());
    CHECK(std::ranges::equal(sut.back(), sut_row2));

    sut.pop_row();
    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size() + sut_row1.size());
    CHECK(std::ranges::equal(sut.back(), sut_row1));

    sut.pop_row();
    CHECK_EQ(sut.n_rows(), 1uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size());
    CHECK(std::ranges::equal(sut.back(), sut_row0));

    sut.pop_row();
    CHECK(sut.empty());
    CHECK_EQ(sut.n_rows(), 0uz);
    CHECK_EQ(sut.n_cols(), 0uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "insert_row should add row at given position") {
    sut.push_row(sut_row0);
    sut.push_row(sut_row2);

    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size() + sut_row2.size());
    CHECK(std::ranges::equal(sut[0uz], sut_row0));
    CHECK(std::ranges::equal(sut[1uz], sut_row2));

    sut.insert_row(1uz, sut_row1);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size() + sut_row1.size() + sut_row2.size());
    CHECK(std::ranges::equal(sut[0uz], sut_row0));
    CHECK(std::ranges::equal(sut[1uz], sut_row1));
    CHECK(std::ranges::equal(sut[2uz], sut_row2));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "insert_row should throw for an invalid position"
) {
    sut.push_row(sut_row0);
    sut.push_row(sut_row1);

    CHECK_THROWS_AS(sut.insert_row(3uz, sut_row2), std::out_of_range);
    CHECK_THROWS_AS(sut.insert_row(10uz, sut_row2), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "insert_row should throw for a not matching row size"
) {
    sut.push_row(sut_row0);
    sut.push_row(sut_row1);

    CHECK_THROWS_AS(sut.insert_row(1uz, {11, 22}), std::invalid_argument);
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "insert_row with value should add new row") {
    sut.push_row({1, 2, 3});
    sut.insert_row(1uz, 4);

    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{4, 4, 4}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "insert_row with value should do nothing if the matrix is empty"
) {
    REQUIRE(sut.empty());

    sut.insert_row(0uz, 0);
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "insert_row with value should throw for invalid position"
) {
    sut.push_row({1, 2, 3});
    CHECK_THROWS_AS(sut.insert_row(2uz, 4), std::out_of_range);
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "erase_row should remove row at given position") {
    sut.push_row(sut_row0);
    sut.push_row(sut_row1);
    sut.push_row(sut_row2);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size() + sut_row1.size() + sut_row2.size());
    CHECK(std::ranges::equal(sut[0uz], sut_row0));
    CHECK(std::ranges::equal(sut[1uz], sut_row1));
    CHECK(std::ranges::equal(sut[2uz], sut_row2));

    sut.erase_row(1uz);

    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_row0.size() + sut_row2.size());
    CHECK(std::ranges::equal(sut[0uz], sut_row0));
    CHECK(std::ranges::equal(sut[1uz], sut_row2));
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "erase_row should throw for an invalid position") {
    sut.push_row(sut_row0);
    sut.push_row(sut_row1);

    CHECK_THROWS_AS(sut.erase_row(2uz), std::out_of_range);
    CHECK_THROWS_AS(sut.erase_row(10uz), std::out_of_range);
}

struct test_flat_matrix_col_modifiers {
    using sut_type = gl::flat_matrix<int>;

    sut_type sut;
    std::vector<int> sut_col0{1, 2, 3};
    std::vector<int> sut_col1{4, 5, 6};
    std::vector<int> sut_col2{7, 8, 9};
};

TEST_CASE_FIXTURE(test_flat_matrix_col_modifiers, "push_col with span should add new column") {
    sut.push_col(std::span<const int>{sut_col0});

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 1uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut.col(0uz), sut_col0));
}

TEST_CASE_FIXTURE(test_flat_matrix_col_modifiers, "push_col with vector should add new column") {
    sut.push_col(sut_col0);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 1uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut.col(0uz), sut_col0));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_col_modifiers, "push_col with initializer list should add new column"
) {
    sut.push_col({1, 2, 3});

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 1uz);
    CHECK_EQ(sut.data_size(), 3uz);
    CHECK(std::ranges::equal(sut.col(0uz), sut_col0));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_col_modifiers, "push_col should throw for a not matching col size"
) {
    sut.push_col(sut_col0);
    sut.push_col(sut_col1);

    CHECK_THROWS_AS(sut.push_col({11, 22}), std::invalid_argument);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_col_modifiers, "multiple push_col calls should add multiple columns"
) {
    sut.push_col(sut_col0);
    sut.push_col(sut_col1);
    sut.push_col(sut_col2);

    CHECK_EQ(sut.size(), 3uz);
    CHECK_EQ(sut.data_size(), sut_col0.size() + sut_col1.size() + sut_col2.size());
    CHECK(std::ranges::equal(sut.col(0uz), sut_col0));
    CHECK(std::ranges::equal(sut.col(1uz), sut_col1));
    CHECK(std::ranges::equal(sut.col(2uz), sut_col2));
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "push_col with value should add new column") {
    sut.push_col({1, 2, 3});
    sut.push_col(4);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 2uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::equal(sut.col(1uz), std::vector<int>{4, 4, 4}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "push_col with value should do nothing if the matrix is empty"
) {
    REQUIRE(sut.empty());

    sut.push_col(0);
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(test_flat_matrix_col_modifiers, "pop_col should remove last column") {
    sut.push_col(sut_col0);
    sut.push_col(sut_col1);

    REQUIRE(std::ranges::equal(sut.back_col(), sut_col1));

    sut.pop_col();

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 1uz);
    CHECK_EQ(sut.data_size(), sut_col0.size());
    CHECK(std::ranges::equal(sut.back_col(), sut_col0));
}

TEST_CASE_FIXTURE(test_flat_matrix_col_modifiers, "pop_col on empty container should do nothing") {
    sut.pop_col();
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(test_flat_matrix_col_modifiers, "pop_col should remove all columns sequentially") {
    sut.push_col(sut_col0);
    sut.push_col(sut_col1);
    sut.push_col(sut_col2);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_col0.size() + sut_col1.size() + sut_col2.size());
    CHECK(std::ranges::equal(sut.back_col(), sut_col2));

    sut.pop_col();
    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 2uz);
    CHECK_EQ(sut.data_size(), sut_col0.size() + sut_col1.size());
    CHECK(std::ranges::equal(sut.back_col(), sut_col1));

    sut.pop_col();
    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 1uz);
    CHECK_EQ(sut.data_size(), sut_col0.size());
    CHECK(std::ranges::equal(sut.back_col(), sut_col0));

    sut.pop_col();
    CHECK(sut.empty());
    CHECK_EQ(sut.n_rows(), 0uz);
    CHECK_EQ(sut.n_cols(), 0uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_col_modifiers, "insert_col should add column at given position") {
    sut.push_col(sut_col0);
    sut.push_col(sut_col2);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 2uz);
    CHECK_EQ(sut.data_size(), sut_col0.size() + sut_col2.size());
    CHECK(std::ranges::equal(sut.col(0uz), sut_col0));
    CHECK(std::ranges::equal(sut.col(1uz), sut_col2));

    sut.insert_col(1uz, sut_col1);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_col0.size() + sut_col1.size() + sut_col2.size());
    CHECK(std::ranges::equal(sut.col(0uz), sut_col0));
    CHECK(std::ranges::equal(sut.col(1uz), sut_col1));
    CHECK(std::ranges::equal(sut.col(2uz), sut_col2));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_col_modifiers, "insert_col should throw for an invalid position"
) {
    sut.push_col(sut_col0);
    sut.push_col(sut_col1);

    CHECK_THROWS_AS(sut.insert_row(4uz, sut_col2), std::out_of_range);
    CHECK_THROWS_AS(sut.insert_row(10uz, sut_col2), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_col_modifiers, "insert_col should throw for a not matching col size"
) {
    sut.push_col(sut_col0);
    sut.push_col(sut_col1);

    CHECK_THROWS_AS(sut.insert_col(1uz, {11, 22}), std::invalid_argument);
}

TEST_CASE_FIXTURE(test_flat_matrix_row_modifiers, "insert_col with value should add new col") {
    sut.push_col({1, 2, 3});
    sut.insert_col(1uz, 4);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 2uz);
    CHECK_EQ(sut.data_size(), 6uz);
    CHECK(std::ranges::equal(sut.col(1uz), std::vector<int>{4, 4, 4}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "insert_col with value should do nothing if the matrix is empty"
) {
    REQUIRE(sut.empty());

    sut.insert_col(0uz, 0);
    CHECK(sut.empty());
}

TEST_CASE_FIXTURE(
    test_flat_matrix_row_modifiers, "insert_col with value should throw for invalid position"
) {
    sut.push_col({1, 2, 3});
    CHECK_THROWS_AS(sut.insert_col(2uz, 4), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_col_modifiers, "erase_col should remove column at given position"
) {
    sut.push_col(sut_col0);
    sut.push_col(sut_col1);
    sut.push_col(sut_col2);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), sut_col0.size() + sut_col1.size() + sut_col2.size());
    CHECK(std::ranges::equal(sut.col(0uz), sut_col0));
    CHECK(std::ranges::equal(sut.col(1uz), sut_col1));
    CHECK(std::ranges::equal(sut.col(2uz), sut_col2));

    sut.erase_col(1uz);

    CHECK_EQ(sut.n_rows(), 3uz);
    CHECK_EQ(sut.n_cols(), 2uz);
    CHECK_EQ(sut.data_size(), sut_col0.size() + sut_col2.size());
    CHECK(std::ranges::equal(sut.col(0uz), sut_col0));
    CHECK(std::ranges::equal(sut.col(1uz), sut_col2));
}

TEST_CASE_FIXTURE(test_flat_matrix_col_modifiers, "erase_col should throw for an invalid position") {
    sut.push_col(sut_col0);
    sut.push_col(sut_col1);

    CHECK_THROWS_AS(sut.erase_col(2uz), std::out_of_range);
    CHECK_THROWS_AS(sut.erase_col(10uz), std::out_of_range);
}

struct test_flat_matrix_complex_operations {
    using sut_type = gl::flat_matrix<int>;
};

TEST_CASE_FIXTURE(
    test_flat_matrix_complex_operations, "interleaved row and col operations should work correctly"
) {
    sut_type sut;

    sut.push_row({1, 2});
    sut.push_col({3});
    sut.insert_row(0uz, {10, 20, 30});
    sut.insert_col(1uz, {15, 25});

    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 4uz);
    CHECK_EQ(sut.data_size(), 8uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{10, 15, 20, 30}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{1, 25, 2, 3}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_complex_operations, "clearing and refilling should work correctly"
) {
    sut_type sut{
        {1, 2, 3},
        {4, 5, 6}
    };

    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 3uz);
    CHECK_EQ(sut.data_size(), 6uz);

    sut.clear();

    CHECK(sut.empty());

    sut.push_row({9, 8});
    sut.push_row({7, 6});

    CHECK_EQ(sut.n_rows(), 2uz);
    CHECK_EQ(sut.n_cols(), 2uz);
    CHECK_EQ(sut.data_size(), 4uz);
    CHECK(std::ranges::equal(sut[0uz], std::vector<int>{9, 8}));
    CHECK(std::ranges::equal(sut[1uz], std::vector<int>{7, 6}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_complex_operations, "large flat_matrix operations should maintain integrity"
) {
    sut_type sut;

    for (int i = 0; i < 100; ++i) {
        std::vector<int> row;
        for (int j = 0; j < 10; ++j)
            row.push_back(i * 10 + j);
        sut.push_row(row);
    }

    CHECK_EQ(sut.n_rows(), 100uz);
    CHECK_EQ(sut.n_cols(), 10uz);
    CHECK_EQ(sut.data_size(), 1000uz);
    for (int i = 0; i < 100; ++i) {
        auto row = sut[static_cast<std::size_t>(i)];
        for (int j = 0; j < 10; ++j)
            CHECK_EQ(row[static_cast<std::size_t>(j)], i * 10 + j);
    }
}

struct test_flat_matrix_iterators {
    using sut_type = gl::flat_matrix<int>;

    sut_type sut{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    std::vector<std::vector<int>> rows{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
};

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "begin() should return iterator to first row") {
    auto it = sut.begin();
    CHECK(std::ranges::equal(*it, rows.front()));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "end() should return iterator past last row") {
    auto it_begin = sut.begin();
    auto it_end = sut.end();
    CHECK_EQ(it_end - it_begin, rows.size());
}

TEST_CASE_FIXTURE(
    test_flat_matrix_iterators, "const begin() should return const iterator to first row"
) {
    const auto& const_sut = sut;
    auto it = const_sut.begin();
    CHECK(std::ranges::equal(*it, rows.front()));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_iterators, "const end() should return const iterator past last row"
) {
    const auto& const_sut = sut;
    auto it_begin = const_sut.begin();
    auto it_end = const_sut.end();
    CHECK_EQ(it_end - it_begin, rows.size());
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "cbegin() should return const iterator") {
    auto it = sut.cbegin();
    CHECK(std::ranges::equal(*it, rows.front()));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "cend() should return const iterator") {
    auto it_begin = sut.cbegin();
    auto it_end = sut.cend();
    CHECK_EQ(it_end - it_begin, rows.size());
}

TEST_CASE_FIXTURE(
    test_flat_matrix_iterators, "non-const iterator should convert to const iterator implicitly"
) {
    auto non_const_it = sut.begin();
    typename sut_type::const_iterator const_it = non_const_it;
    CHECK(std::ranges::equal(*const_it, rows.front()));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "dereferencing iterator should return row") {
    auto it = sut.begin();
    CHECK(std::ranges::equal(*it, rows[0uz]));

    ++it;
    CHECK(std::ranges::equal(*it, rows[1uz]));

    ++it;
    CHECK(std::ranges::equal(*it, rows[2uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "operator[] should access row at offset") {
    auto it = sut.begin();
    CHECK(std::ranges::equal(it[0uz], rows[0uz]));
    CHECK(std::ranges::equal(it[1uz], rows[1uz]));
    CHECK(std::ranges::equal(it[2uz], rows[2uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "pre-increment should advance iterator") {
    auto it = sut.begin();
    CHECK(std::ranges::equal(*it, rows[0uz]));

    auto& ret = ++it;
    CHECK(std::ranges::equal(*ret, rows[1uz]));
    CHECK(std::ranges::equal(*it, rows[1uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "post-increment should return old iterator") {
    auto it = sut.begin();
    CHECK(std::ranges::equal(*it, rows[0uz]));

    auto old_it = it++;
    CHECK(std::ranges::equal(*old_it, rows[0uz]));
    CHECK(std::ranges::equal(*it, rows[1uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "pre-decrement should move iterator backward") {
    auto it = sut.end();
    --it;
    CHECK(std::ranges::equal(*it, rows[2uz]));

    auto& ret = --it;
    CHECK(std::ranges::equal(*ret, rows[1uz]));
    CHECK(std::ranges::equal(*it, rows[1uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "post-decrement should return old iterator") {
    auto it = sut.end();
    --it;
    CHECK(std::ranges::equal(*it, rows[2uz]));

    auto old_it = it--;
    CHECK(std::ranges::equal(*old_it, rows[2uz]));
    CHECK(std::ranges::equal(*it, rows[1uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "operator+= should advance iterator") {
    auto it = sut.begin();
    it += 2;
    CHECK(std::ranges::equal(*it, rows[2uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "operator-= should move iterator backward") {
    auto it = sut.end();
    it -= 1;
    CHECK(std::ranges::equal(*it, rows[2uz]));

    it -= 2;
    CHECK(std::ranges::equal(*it, rows[0uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "operator+ should create new iterator") {
    auto it = sut.begin();
    auto new_it = it + 1;

    CHECK(std::ranges::equal(*it, rows[0uz]));
    CHECK(std::ranges::equal(*new_it, rows[1uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "reverse operator+ should create new iterator") {
    auto it = sut.begin();
    auto new_it = 2 + it;

    CHECK(std::ranges::equal(*it, rows[0uz]));
    CHECK(std::ranges::equal(*new_it, rows[2uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "operator- should create new iterator") {
    auto it = sut.end();
    auto new_it = it - 1;

    CHECK(std::ranges::equal(*new_it, rows[2uz]));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "operator- with two iterators should give distance") {
    auto it1 = sut.begin();
    auto it2 = sut.end();

    CHECK_EQ(it2 - it1, sut.n_rows());
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "operator== should compare iterators") {
    auto it1 = sut.begin();
    auto it2 = sut.begin();
    auto it3 = sut.begin() + 1;

    CHECK_EQ(it1, it2);
    CHECK_NE(it1, it3);
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "operator<=> should compare iterators") {
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

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "range-based for loop should iterate all rows") {
    std::size_t idx = 0uz;
    for (auto row : sut)
        CHECK(std::ranges::equal(row, rows[idx++]));
    CHECK_EQ(idx, 3uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "const range-based for loop should iterate all rows") {
    const auto& const_sut = sut;
    std::size_t idx = 0uz;
    for (auto row : const_sut)
        CHECK(std::ranges::equal(row, rows[idx++]));
    CHECK_EQ(idx, 3uz);
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "rbegin() should return reverse iterator") {
    auto it = sut.rbegin();
    CHECK(std::ranges::equal(*it, rows.back()));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "rend() should return reverse iterator past first") {
    auto it_rbegin = sut.rbegin();
    auto it_rend = sut.rend();
    CHECK_EQ(it_rend - it_rbegin, rows.size());
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "crbegin() should return const reverse iterator") {
    auto it = sut.crbegin();
    CHECK(std::ranges::equal(*it, rows.back()));
}

TEST_CASE_FIXTURE(test_flat_matrix_iterators, "crend() should return const reverse iterator") {
    auto it_rbegin = sut.crbegin();
    auto it_rend = sut.crend();
    CHECK_EQ(it_rend - it_rbegin, rows.size());
}

TEST_CASE_FIXTURE(
    test_flat_matrix_iterators, "reverse range-based for loop should iterate in reverse"
) {
    std::size_t idx = 2uz;
    for (auto it : std::ranges::reverse_view(sut)) {
        CHECK(std::ranges::equal(it, rows[idx]));
        if (idx > 0uz)
            --idx;
    }
}

struct test_flat_matrix_transformations {
    using sut_type = gl::flat_matrix<int>;
};

TEST_CASE_FIXTURE(
    test_flat_matrix_transformations, "transpose should correctly transpose a square matrix"
) {
    sut_type sut{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    auto transposed = sut.transpose();

    CHECK_EQ(transposed.n_rows(), 3uz);
    CHECK_EQ(transposed.n_cols(), 3uz);
    CHECK_EQ(transposed.data_size(), 9uz);
    CHECK(std::ranges::equal(transposed[0uz], std::vector<int>{1, 4, 7}));
    CHECK(std::ranges::equal(transposed[1uz], std::vector<int>{2, 5, 8}));
    CHECK(std::ranges::equal(transposed[2uz], std::vector<int>{3, 6, 9}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_transformations, "transpose should correctly transpose a rectangular matrix"
) {
    sut_type sut{
        {1, 2, 3, 4},
        {5, 6, 7, 8}
    };

    auto transposed = sut.transpose();

    CHECK_EQ(transposed.n_rows(), 4uz);
    CHECK_EQ(transposed.n_cols(), 2uz);
    CHECK_EQ(transposed.data_size(), 8uz);
    CHECK(std::ranges::equal(transposed[0uz], std::vector<int>{1, 5}));
    CHECK(std::ranges::equal(transposed[1uz], std::vector<int>{2, 6}));
    CHECK(std::ranges::equal(transposed[2uz], std::vector<int>{3, 7}));
    CHECK(std::ranges::equal(transposed[3uz], std::vector<int>{4, 8}));
}

TEST_CASE_FIXTURE(
    test_flat_matrix_transformations, "transpose on an empty matrix should return an empty matrix"
) {
    sut_type sut;

    auto transposed = sut.transpose();

    CHECK(transposed.empty());
    CHECK_EQ(transposed.n_rows(), 0uz);
    CHECK_EQ(transposed.n_cols(), 0uz);
    CHECK_EQ(transposed.data_size(), 0uz);
}

TEST_SUITE_END(); // test_flat_matrix

} // namespace gl_testing
