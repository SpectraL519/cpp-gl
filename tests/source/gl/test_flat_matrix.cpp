#include <gl/types/flat_matrix.hpp>

#include <doctest.h>

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

    sut = std::move(sut);

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

    std::vector<std::vector<int>> data{sut_row0, sut_row1};
    std::vector<int> flat_data{1, 2, 3, 4, 5, 6};

    std::size_t sut_n_rows = 2uz;
    std::size_t sut_n_cols = 3uz;

    int dummy_value = 111;
};

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "operator[r] should return row at given index") {
    auto r0 = sut[0uz];
    CHECK(std::ranges::equal(r0, data[0uz]));
    r0.front() = dummy_value;
    CHECK_EQ(sut[0uz].front(), dummy_value);

    auto r1 = sut[1uz];
    CHECK(std::ranges::equal(r1, data[1uz]));
    r1.front() = dummy_value;
    CHECK_EQ(sut[1uz].front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const operator[r] should return const row at given index"
) {
    const auto& const_sut = sut;

    auto r0 = const_sut[0uz];
    CHECK(std::ranges::equal(r0, data[0uz]));

    auto r1 = const_sut[1uz];
    CHECK(std::ranges::equal(r1, data[1uz]));
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
    CHECK(std::ranges::equal(r0, data[0uz]));
    r0.front() = dummy_value;
    CHECK_EQ(sut.at(0uz).front(), dummy_value);

    auto r1 = sut.at(1uz);
    CHECK(std::ranges::equal(r1, data[1uz]));
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
    CHECK(std::ranges::equal(r0, data[0uz]));

    auto r1 = const_sut.at(1uz);
    CHECK(std::ranges::equal(r1, data[1uz]));
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

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "rows() should return a view of all rows") {
    auto n_rows = 0uz;
    for (auto row : sut.rows()) {
        CHECK(std::ranges::equal(row, data[n_rows]));
        const auto orig_val = std::exchange(row.front(), dummy_value);
        CHECK_EQ(sut[n_rows].front(), dummy_value);
        row.front() = orig_val; // revert change

        n_rows++;
    }

    CHECK_EQ(n_rows, sut_n_rows);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "front() should return the first row") {
    auto front_row = sut.front();
    CHECK(std::ranges::equal(front_row, data.front()));

    front_row.front() = dummy_value;
    CHECK_EQ(sut.front().front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const front() should return const first row") {
    const auto& const_sut = sut;
    auto front_row = const_sut.front();
    CHECK(std::ranges::equal(front_row, data.front()));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "front(row) should return first element in row") {
    CHECK_EQ(sut.front(0uz), sut_row0.front());
    sut.front(0uz) = dummy_value;
    CHECK_EQ(sut[0uz].front(), dummy_value);

    CHECK_EQ(sut.front(1uz), sut_row1.front());
    sut.front(1uz) = dummy_value;
    CHECK_EQ(sut[1uz].front(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const front(row) should return const first element in row"
) {
    const auto& const_sut = sut;
    CHECK_EQ(const_sut.front(0uz), sut_row0.front());
    CHECK_EQ(const_sut.front(1uz), sut_row1.front());
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "back() should return the last row") {
    auto back_row = sut.back();
    CHECK(std::ranges::equal(back_row, data.back()));

    back_row.front() = dummy_value;
    CHECK_EQ(sut.back().front(), dummy_value);
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "const back() should return const last row") {
    const auto& const_sut = sut;
    auto back_row = const_sut.back();
    CHECK(std::ranges::equal(back_row, data.back()));
}

TEST_CASE_FIXTURE(test_flat_matrix_accessors, "back(row) should return last element in row") {
    CHECK_EQ(sut.back(0uz), sut_row0.back());
    sut.back(0uz) = dummy_value;
    CHECK_EQ(sut[0uz].back(), dummy_value);

    CHECK_EQ(sut.back(1uz), sut_row1.back());
    sut.back(1uz) = dummy_value;
    CHECK_EQ(sut[1uz].back(), dummy_value);
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const back(row) should return const last element in row"
) {
    const auto& const_sut = sut;
    CHECK_EQ(const_sut.back(0uz), sut_row0.back());
    CHECK_EQ(const_sut.back(1uz), sut_row1.back());
}

TEST_CASE_FIXTURE(
    test_flat_matrix_accessors, "const rows() should return a const view of all rows"
) {
    const auto& const_sut = sut;

    auto n_rows = 0uz;
    for (auto row : const_sut.rows()) {
        CHECK(std::ranges::equal(row, data[n_rows]));
        n_rows++;
    }

    CHECK_EQ(n_rows, sut_n_rows);
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

TEST_SUITE_END(); // test_flat_matrix

} // namespace gl_testing
