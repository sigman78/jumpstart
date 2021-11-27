//
//

#include <doctest/doctest.h>

// #define PLATFORM_NO_ASSERT

#include "platform/assert.h"

TEST_CASE("assert compile test") {
    // should compile and run
    int val = 1;
    PLATFORM_ASSERT(val == 1, "This should not fail");

    switch(val) {
        case 1:
            break;
        default:
            PLATFORM_ASSERT_UNREACHABLE();
    }
}

