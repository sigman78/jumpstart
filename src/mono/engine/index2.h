#pragma once

#include <vector>
#include <ostream>
#include <limits>
#include <algorithm>
#include <stdlib.h>
#include <tuple>

namespace mono {

    using index = int;

    struct index2 {
        //TODO: discuss and make less ugly
        index x = -65536;//std::numeric_limits<int>::max();
        index y = -65536;

        static std::vector<index2> const unit;
        index magnitude() const;
        static index distance(const index2&, const index2&);

        operator bool() const { return operator!=({}); }
        index2 operator+(const index2& rhs) const { return {x+rhs.x, y+rhs.y}; }
        index2 operator-(const index2& rhs) const { return {x-rhs.x, y-rhs.y}; }
        index2 operator-() const { return {-x, -y}; }
        bool operator==(const index2& rhs) const { return (x == rhs.x) && (y == rhs.y); }
        bool operator!=(const index2& rhs) const { return not operator==(rhs); }

        // non-2D comparison (for sorting purposes)
        bool operator<(const index2& rhs) const { return std::tie(y, x) < std::tie(rhs.y, rhs.x); }
    };

    //impl:
    inline std::vector<index2> const index2::unit{{1, 0}, {1,1}, {0,1}, {-1,0}, {-1,-1}, {0, -1}};

    inline index index2::magnitude() const
    {
        if (x == 0) return abs(y);
        if (y == 0) return abs(x);
        if (x * y > 0)
        {
            return abs(x) + abs(y);
        }
        else
        {
            return std::max(abs(x), abs(y));
        }
    }

    inline index index2::distance(const index2& lhs, const index2& rhs)
    {
        return index2{ rhs.x - lhs.x, rhs.y - lhs.y }.magnitude();
    }

    //io
    inline std::ostream& operator<<(std::ostream& s, const index2& rhs)
    {
        s << "{" << rhs.x << ":" << rhs.y << "}";
        return s;
    }
}