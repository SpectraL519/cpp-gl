#include "testing/gl/alg_common.hpp"
#include "testing/gl/constants.hpp"

#include <gl/algorithm/types.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

using gl::algorithm::predicate_result;

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_types");

TEST_CASE("predicate_result(bool) should properly initialize the value") {
    predicate_result result(true);
    CHECK_EQ(result, predicate_result::ok);

    predicate_result result2(false);
    CHECK_EQ(result2, predicate_result::not_ok);
}

TEST_CASE("predicate_result bool conversion should return true only for ok eval") {
    CHECK(predicate_result{predicate_result::ok});
    CHECK_FALSE(predicate_result{predicate_result::not_ok});
    CHECK_FALSE(predicate_result{predicate_result::unknown});
}

TEST_SUITE_END(); // test_alg_types

} // namespace gl_testing
