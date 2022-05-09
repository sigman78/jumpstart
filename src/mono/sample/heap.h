#pragma once

#include <mono/engine/Database.h>
#include <mono/engine/index2.h>


namespace sample {

    using namespace mono;

    struct Clock : Requires<World> {
        struct Item {
            int day=0;
        };
        using Data = container::Unique<Item>;
    };

    struct HexType : Requires<World> {
        struct Item {
            std::string name;
        };
        using Data = container::Table<const Item>;
    };


        
    };
} // namespace

