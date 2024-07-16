#include <doctest.h>
#include <fakeit.hpp>

#include <concepts>
#include <utility>

using namespace fakeit;

/*
This test file was created to determine whether the doctest and fakeit libraries
and testing CMakeLists.txt file have been set up correctly :)
*/

TEST_SUITE_BEGIN("test_dectest_config");

TEST_CASE("true should be true") {
    REQUIRE(true);
}

TEST_CASE("true should not be false") {
    REQUIRE_FALSE(false);
}

TEST_SUITE_END();

TEST_SUITE_BEGIN("test_fakeit_config");

namespace {

template <typename T>
concept c_config_mock = requires(T instance) {
    { instance.foo() } -> std::same_as<int>;
    { instance.bar(std::declval<const int>()) } -> std::same_as<double>;
};

class MockInterface {
public:
    virtual int foo() const = 0;
    virtual double bar(const int) const = 0;
};

} // namespace

static_assert(c_config_mock<MockInterface>, "MockInterface does not satisfy c_config_mock");

TEST_CASE("mock should perform actions correctly") {
    constexpr int foo_value = 3;
    constexpr double bar_value = foo_value * 0.5;

    Mock<MockInterface> mock;
    When(Method(mock, foo)).Return(foo_value);
    When(Method(mock, bar).Using(foo_value)).AlwaysReturn(bar_value);

    const auto& mock_instance = mock.get();

    CHECK_EQ(mock_instance.foo(), foo_value);
    CHECK_EQ(mock_instance.bar(foo_value), bar_value);
    CHECK_NE(mock_instance.bar(foo_value), foo_value);

    Verify(Method(mock, foo)).Exactly(1);
    Verify(Method(mock, bar).Using(foo_value)).AtLeast(1);
}

TEST_SUITE_END();
