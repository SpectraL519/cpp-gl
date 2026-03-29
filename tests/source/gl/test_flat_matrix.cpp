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

TEST_SUITE_END(); // test_flat_matrix

} // namespace gl_testing
