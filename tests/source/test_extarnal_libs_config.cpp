#include <doctest.h>
#include <fakeit.hpp>

#include <concepts>
#include <utility>

using namespace fakeit;

/*
This test file was created to determine whether the doctest and fakeit libraries
and testing CMakeLists.txt file have been set up correctly :)
*/

namespace gl_testing {

TEST_SUITE_BEGIN("test_doctest_config");

namespace {

int add(int a, int b) {
    return a + b;
}

float multiply(float a, float b) {
    return a * b;
}

int divide(const int numerator, const int denominator) {
    if (not denominator)
        throw std::logic_error("Division by zero");
    return numerator / denominator;
}

} // namespace

TEST_CASE("addition of integers") {
    int a = 5;
    int b = 10;
    CHECK(add(a, b) == 15);
}

TEST_CASE("multiplication of floats") {
    constexpr float x = 2.5f;
    constexpr float y = 3.0f;
    CHECK(multiply(x, y) == doctest::Approx(7.5).epsilon(0.01));
}

TEST_CASE("division by zero") {
    int numerator = 10;
    int denominator = 0;
    CHECK_THROWS_AS(divide(numerator, denominator), std::logic_error);
}

TEST_SUITE_END(); // test_doctest_config

TEST_SUITE_BEGIN("test_fakeit_config");

namespace {

template <typename T>
concept c_config_test_type = requires(T instance) {
    { instance.foo() } -> std::same_as<int>;
    { instance.bar(std::declval<const int>()) } -> std::same_as<double>;
};

class ConfigTestInterface {
public:
    virtual int foo() const = 0;
    virtual double bar(const int) const = 0;
};

template <c_config_test_type ConfigTestType>
class TestClass {
public:
    TestClass(const ConfigTestType& conifg_test_object) : _conifg_test_object(conifg_test_object) {}

    int get_foo() const {
        return this->_conifg_test_object.foo();
    }

    double get_bar(const int foo) const {
        return this->_conifg_test_object.bar(foo);
    }

private:
    const ConfigTestType& _conifg_test_object;
};

} // namespace

TEST_CASE("mock should perform actions correctly") {
    constexpr int foo_value = 3;
    constexpr double bar_value = foo_value * 0.5;

    Mock<ConfigTestInterface> config_test_mock;
    When(Method(config_test_mock, foo)).Return(foo_value);
    When(Method(config_test_mock, bar).Using(foo_value)).AlwaysReturn(bar_value);

    const auto& config_mock_instance = config_test_mock.get();
    const TestClass test_object{config_mock_instance};

    CHECK_EQ(test_object.get_foo(), foo_value);
    CHECK_EQ(test_object.get_bar(foo_value), bar_value);
    CHECK_NE(test_object.get_bar(foo_value), foo_value);

    Verify(Method(config_test_mock, foo)).Exactly(1);
    Verify(Method(config_test_mock, bar).Using(foo_value)).Exactly(2);
}

TEST_SUITE_END(); // test_fakeit_config

} // namespace gl_testing
