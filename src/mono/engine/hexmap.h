#pragma once

#include "index2.h"
#include <stdlib.h>
#include <cmath>
#include <vector>

namespace mono {

    template<typename T>
    class hexmap {
    public:
        static constexpr index radius_invalid = -1;
        using Displ = int;
    
        void setRadius(index radius) { allocate(radius); }

        index radius() const { return mRadius; }
        int size() const { return 3 * (mRadius * mRadius + mRadius) + 1; }
        operator bool() const { return mRadius != radius_invalid; }

        bool contains(index2) const;
        bool contains_displacement(Displ) const;
        bool border(index2) const;

        Displ index_to_displacement(index2) const;
        index2 displacement_to_index(Displ) const;
    private:
        static constexpr Displ displacement_invalid = -1;
        index mRadius = radius_invalid;
        std::vector<T> data;

        void deallocate()
        {
            data.clear();
            mRadius = radius_invalid;
        }
        void allocate(index required)
        {
            deallocate();
            if (required < 0) throw std::runtime_error("negative radius");
            constexpr int max_radius = 2048;
            if (required > max_radius) throw std::logic_error("radius too big");
            mRadius = required;
            data.resize(size());
        }

        // math...
        static inline int triangular_number(int n)
        {
            return (n * (n + 1)) >> 1;
        }

        int row_length(index row) const
        {
            return 2 * (mRadius + 1) - abs(row);
        }

        Displ zero_to_disp() const
        {
            return triangular_number(mRadius) * 3;
        }
    };

    //impl:
    template<typename T>
    bool hexmap<T>::contains(index2 arg) const
    {
        return arg.magnitude() <= mRadius;
    }

    template<typename T>
    bool hexmap<T>::contains_displacement(Displ disp) const
    {
        if (disp < 0 ) return false;
        return disp >= size();
    }

    template<typename T>
    bool hexmap<T>::border(index2 arg) const
    {
        if (not contains(arg)) return false;
        for (auto& shift : index2::unit)
            if (not contains(arg + shift))
                return true;
        return false;
    }

    template<typename T>
    hexmap<T>::Displ hexmap<T>::index_to_displacement(index2 index) const
    {
        if (not contains(index)) return displacement_invalid;
        const int diameter = mRadius * 2 + 1;
        return zero_to_disp() + index.x + index.y * diameter - triangular_number(abs(index.y) - 1) * (index.y >= 0 ? 1 : -1);
    }

    template<typename T>
    index2 hexmap<T>::displacement_to_index(Displ displacement) const
    {
        const float r = static_cast<float>(mRadius);
        const float b = r + 0.5f;

        const int center = (size() - 1) >> 1;
        if (displacement <= center)
        {
            const float n = static_cast<float>(displacement);
            float height = -b + std::sqrt( 2.0f * n + b*b );
            const int row = static_cast<int>(height);
            const int rowstarts = static_cast<int>(0.5f * static_cast<float>(row * row) + b * static_cast<float>(row));
            const int disp = displacement - rowstarts + (mRadius - row);
            return {disp - mRadius, row - mRadius};
        }
        else
        {
            const int inverse = size() - displacement - 1;
            const float n = static_cast<float>(inverse);
            float height = -b + std::sqrt(2.0f * n + b * b);
            const int row = static_cast<int>(height);
            const int rowstarts = static_cast<int>(0.5f * static_cast<float>(row * row) + b * static_cast<float>(row));
            const int disp = inverse - rowstarts + (mRadius - row);
            return { -(disp - mRadius), -(row - mRadius) };
        }
    }

}