#pragma once

#include "Vec3.h"


class TexVertex
{
public:

    TexVertex InterpolateTo(TexVertex const& dest, float alpha)
    {
        return
        TexVertex
        {
            interpolate(model_pos, dest.model_pos, alpha),
            interpolate(texture_pos, dest.texture_pos, alpha)
        };
    }

    TexVertex& operator+=(TexVertex const& rhs)
    {
        model_pos += rhs.model_pos;
        texture_pos += rhs.texture_pos;
        
        return *this;
    }
    TexVertex& operator-=(TexVertex const& rhs)
    {
        model_pos -= rhs.model_pos;
        texture_pos -= rhs.texture_pos;

        return *this;
    }

    TexVertex& operator*=(float const& rhs)
    {
        model_pos *= rhs;
        texture_pos *= rhs;

        return *this;
    }
    TexVertex& operator/=(float const& rhs)
    {
        model_pos /= rhs;
        texture_pos /= rhs;

        return *this;
    }

    Vec3 model_pos{ };
    Vec2 texture_pos{ };
};

inline TexVertex operator+(TexVertex const& lhs, TexVertex const& rhs)
{
    return TexVertex{ lhs } += rhs;
}
inline TexVertex operator-(TexVertex const& lhs, TexVertex const& rhs)
{
    return TexVertex{ lhs } -= rhs;
}

inline TexVertex operator*(TexVertex const& lhs, float const& rhs)
{
    return TexVertex{ lhs } *= rhs;
}
inline TexVertex operator/(TexVertex const& lhs, float const& rhs)
{
    return TexVertex{ lhs } /= rhs;
}

inline TexVertex operator*(float const& lhs, TexVertex const& rhs)
{
    return TexVertex{ rhs } *= lhs;
}
inline TexVertex operator/(float const& lhs, TexVertex const& rhs)
{
    return TexVertex{ rhs } /= lhs;
}