#include "testing/gl/alg_utils.hpp"
#include "testing/gl/constants.hpp"

#include <gl/algorithm/core.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

using gl::algorithm::decision;

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_types");

TEST_CASE("decision(bool) should properly initialize the value") {
    decision result(true);
    CHECK_EQ(result, decision::accept);

    decision result2(false);
    CHECK_EQ(result2, decision::reject);
}

TEST_CASE("decision bool conversion should return true only for ok eval") {
    CHECK(decision{decision::accept});
    CHECK_FALSE(decision{decision::reject});
    CHECK_FALSE(decision{decision::abort});
}

TEST_SUITE_END(); // test_alg_types

} // namespace gl_testing
