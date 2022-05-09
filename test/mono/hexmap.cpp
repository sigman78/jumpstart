#include <doctest/doctest.h>

#include <mono/engine/hexmap.h>
#include <functional>

TEST_CASE("hexmap::directions")
{
    using namespace mono;
    const index2 origin{7, 3};
    index2 cursor = origin;

    for (int direction = 0; direction < 6; ++direction)
        cursor = cursor + index2::unit[direction];

    CHECK(cursor == origin);
}

TEST_CASE("hexmap::measurement")
{
    using namespace mono;
    { // central hexagonal numbers...
        hexmap<int> map;
        map.setRadius(7);
        CHECK(map.size() == 169);
    }

    { // borders, contains, e.t.c
        hexmap<int> map;
        map.setRadius(2);
        CHECK(map.size() == 19);
        CHECK(map.contains({ -2,0 }));
        CHECK(not map.contains({ -2,-1 }));
        CHECK(map.border({ -1,-1 }));
        CHECK(not map.border({ -1,11 }));
    }
    { // displacements and indices:
        hexmap<int> map;
        map.setRadius(4);

        auto verify = [&map](index2 idx, int disp) {
            CHECK(map.displacement_to_index(disp) == idx);
            CHECK(map.index_to_displacement(idx) == disp);
        };

        const std::vector<std::pair<index2, int>> table{
            {{0,0}, 30},
            {{3,0}, 33},
            {{2,-2}, 15},
            {{-4,1}, 35},
            {{-1,4}, 59},
            {{-1,-3}, 5},
        };
        for (auto& it : table)
            verify(it.first, it.second);
    }
    { // conversions total test:
        const std::vector<int> sizeCases{1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
        for (int scase = 0; scase < sizeCases.size(); ++scase)
        {
            const int size = sizeCases[scase];
            hexmap<int> map;
            map.setRadius(size);

            for (int disp = 0; disp < map.size(); ++disp)
            {
                CHECK(map.index_to_displacement(map.displacement_to_index(disp)) == disp);
            }
        }
    }
}
