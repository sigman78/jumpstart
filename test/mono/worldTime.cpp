#include <doctest/doctest.h>

#include <mono/world/Time.h>

TEST_CASE("worldtime::math")
{
    mono::world::Time::Item time;
    time.absolute = 392;
    CHECK(time.day_of_year() == 32);
}

