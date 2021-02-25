#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <debug.h>

#include <iostream>

#include <vector>
#include <map>

TEST_CASE("basic output") {
    {
        std::ostringstream out;
        Platform::Debug(&out) << "1. 2. 3. Fail!";
        CHECK(out.str() == "1. 2. 3. Fail!\n");
    }
    {
        std::ostringstream out;
        !Platform::Debug(&out) << "1. 2. 3. Fail!"; // fix if line moved :)
        CHECK(out.str() == __FILE__  ":18 " "1. 2. 3. Fail!\n");
    }
}

TEST_CASE("basic types sanity") {
    using Platform::Debug;
    std::ostringstream null;
    
    Debug(&null) << 'a';
    Debug(&null) << "aa";
    Debug(&null) << 1;
    Debug(&null) << 2U;
    Debug(&null) << 3L;
    Debug(&null) << 4UL;
    Debug(&null) << 5LL;
    Debug(&null) << 6ULL;
    Debug(&null) << 3.14f;
    Debug(&null) << 3.1415;
    Debug(&null) << (void*)(0xaa);
    Debug(&null) << nullptr;
    Debug(&null) << true;

    CHECK(null.str() == "97\naa\n1\n2\n3\n4\n5\n6\n3.14\n3.1415\n0xaa\nnull\ntrue\n");
}

TEST_CASE("functionality") {
    Platform::Debug() << "Hello" << "world" << Platform::Debug::nosp << "!";
    Platform::Warning() << "You've got" << 1 << "problem.";
    Platform::Error() << "And you have to stop" << "!";
}

TEST_CASE("spaced output") {
    using Platform::Debug;

    {
        std::ostringstream out;
        Debug(&out) << "1" << 2;
        CHECK(out.str() == "1 2\n");
    }
    {
        std::ostringstream out;
        Debug(&out) << "1" << Debug::nosp << 2;
        CHECK(out.str() == "12\n");
    }
    {
        std::ostringstream out;
        Debug(&out) << Debug::nonl << "1" << Debug::nosp << 2;
        CHECK(out.str() == "12");
    }
}

TEST_CASE("warning output") {
    using Platform::Warning;

    {
        std::ostringstream out;
        Warning(&out) << "1" << 2;
        CHECK(out.str() == "1 2\n");
    }
    {
        std::ostringstream out;
        Warning(&out) << Warning::nonl << 1 << Warning::nosp << "2";
        CHECK(out.str() == "12");
    }
}

TEST_CASE("error output") {
    using Platform::Error;

    {
        std::ostringstream out;
        Error(&out) << "1" << 2;
        CHECK(out.str() == "1 2\n");
    }
    {
        std::ostringstream out;
        Error(&out) << Error::nonl << 1 << Error::nosp << "2";
        CHECK(out.str() == "12");
    }
}

struct foo {};
std::ostream& operator << (std::ostream& out, const foo& f) {
    out << "<foo>";
    return out;
}

TEST_CASE("ostream enabled types") {
    using Platform::Debug;

    {
        std::ostringstream out;
        out << foo{}; // sanity check
        Debug(&out) << foo{} << Debug::nonl;
        CHECK(out.str() == "<foo><foo>");
    }
}

TEST_CASE("pair compatibility") {
    using Platform::Debug;

    {
        std::ostringstream out;
        std::pair<std::string, long> p{"111", 2};
        Debug(&out) << p << Debug::nonl;
        CHECK(out.str() == "(111, 2)");
    }
}

TEST_CASE("simple iterable") {
    using Platform::Debug;

    {
        std::ostringstream out;
        std::vector<int> v = {1, 2, 3};
        static_assert(Platform::IsIterable<decltype(v)>::value, "sanity: IsIterable should work");
        static_assert(!Platform::IsString<decltype(v)>::value, "sanity: IsString should not work");
        Debug(&out) << v << Debug::nonl;
        CHECK(out.str() == "{1, 2, 3}");
    }
}

TEST_CASE("nested vector") {
    using Platform::Debug;

    {
        std::ostringstream out;
        std::vector<std::vector<int>> v = {{1}, {2, 3}};
        static_assert(Platform::IsIterable<decltype(v)>::value, "sanity: IsIterable should work");
        static_assert(!Platform::IsString<decltype(v)>::value, "sanity: IsString should not work");
        Debug(&out) << v << Debug::nonl;
        CHECK(out.str() == "{{1}, {2, 3}}");
    }
}

TEST_CASE("map iterable") {
    using Platform::Debug;

    {
        std::ostringstream out;
        std::map<int, std::string> m = {{1, "asd"}, {2, "bcd"}};
        static_assert(Platform::IsIterable<decltype(m)>::value, "sanity: IsIterable should work");
        static_assert(!Platform::IsString<decltype(m)>::value, "sanity: IsString should not work");
        Debug(&out) << m << Debug::nonl;
        CHECK(out.str() == "{(1, asd), (2, bcd)}");
    }
}

TEST_CASE("map of vectors") {
    using Platform::Debug;

    {
        std::ostringstream out;
        std::map<int, std::vector<int>> m = {{1, {2, 3}}, {2, {5, 6}}};
        static_assert(Platform::IsIterable<decltype(m)>::value, "sanity: IsIterable should work");
        static_assert(!Platform::IsString<decltype(m)>::value, "sanity: IsString should not work");
        Debug(&out) << m << Debug::nonl;
        CHECK(out.str() == "{(1, {2, 3}), (2, {5, 6})}");
    }
}
