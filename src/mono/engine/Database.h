#pragma once

#include <mono/engine/index2.h>
#include <mono/engine/hexmap.h>
#include <mono/engine/TypeInfo.h>
#include <map>
#include <memory>
#include <utility>
#include <iostream>

namespace mono {

    template<class... Types>
    struct Requires {
        //static std::tuple<Types> required;
        using DeclaredDependencies = std::tuple<Types...>;
    };

    // identifier type is to be unique for every type of data.
    struct container {
    private:
        template <class BaseType, typename _UniqueTag>
        struct IdentifierBase {
            IdentifierBase() : value({}) {}
            IdentifierBase(const BaseType& arg) : value(arg) {}
            inline bool operator==(const IdentifierBase& rhs) const { return value == rhs.value; }
            inline bool operator!=(const IdentifierBase& rhs) const { return value != rhs.value; }
            inline bool operator<(const IdentifierBase& rhs) const { return value < rhs.value; }
            inline operator bool() const { return value; }
        private:
            BaseType value = {};
        };
    public:
        template<typename ItemType>
        struct Unique {
            using Identifier = IdentifierBase<bool, ItemType>;
            using Container = std::array<ItemType, 1>;
            static bool contains(const Container&, Identifier arg) { return arg; }
        };

        template<typename ItemType>
        struct Table {
            using Identifier = IdentifierBase<int, ItemType>;
            using Container = std::map<Identifier, ItemType>;
            static bool contains(const Container& cont, Identifier arg) { return cont.count(arg); }
        };

        template<typename ItemType>
        struct Map {
            using Identifier = IdentifierBase<index2, ItemType>;
            using Container = mono::hexmap<ItemType>;
            static bool contains(const Container& cont, Identifier arg ) { return cont.contains(arg); }
        };
    };

    struct LayerBase {
        virtual ~LayerBase() {}
    };

    // this final class is "set of objects" defined by "Type"
    template<typename Type>
    struct Layer : LayerBase {
        using Id = typename Type::Data::Identifier;
        using Item = typename Type::Item;
        using Container = typename Type::Data::Container;

        Container items;
    };

    class Root {
    public:
        template<typename T>
        const typename Layer<T>::Container& layer();

        template<typename T>
        const typename Layer<T>::Item& item(typename Layer<T>::Id);
    private:
        std::map<RuntimeTypeId, std::shared_ptr<LayerBase>> mLayers;
    };

    // test (remove)
    
    struct Test : Requires<Root> {
        struct Item {
            int x;
            std::string name;
        };
        using Data = container::Table<Item>;
    };

    inline void dummy()
    {
        Root world;
        std::cout << world.layer<Test>().size() << std::endl;
        std::cout << world.item<Test>(7).name << std::endl;
        Test::Item change = world.item<Test>(7);
        { // Scoped change:
     //       auto change = 
        }
    }
    //-


    //impl:
    template<typename Name>
    const typename Layer<Name>::Container& Root::layer()
    {
        const RuntimeTypeId required = getRuntimeTypeId<Name>();
        std::shared_ptr<LayerBase>& res = mLayers[required];
        if (not res)
            res = std::make_shared<Layer<Name>>();

        return std::dynamic_pointer_cast<Layer<Name>>(res)->items;
    }

    template<typename Name>
    const typename Layer<Name>::Item& Root::item(Layer<Name>::Id id)
    {
        return layer<Name>().at(id);
    }
}