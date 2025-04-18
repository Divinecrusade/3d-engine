#pragma once

#include <utility>
#include "Vec3.h"

class TextureBindedVertex
{
public:

    TextureBindedVertex(Vec3 model, Vec2 texture = { 0.f, 0.f })
        :
        model_pos{ std::move(model) },
        texture_pos{ std::move(texture) }
    { }

    TextureBindedVertex& operator+=(TextureBindedVertex const& rhs)
    {
        model_pos += rhs.model_pos;
        texture_pos += rhs.texture_pos;

        return *this;
    }
    TextureBindedVertex& operator-=(TextureBindedVertex const& rhs)
    {
        model_pos -= rhs.model_pos;
        texture_pos -= rhs.texture_pos;

        return *this;
    }

    TextureBindedVertex& operator*=(float const& rhs)
    {
        model_pos *= rhs;
        texture_pos *= rhs;

        return *this;
    }
    TextureBindedVertex& operator/=(float const& rhs)
    {
        model_pos /= rhs;
        texture_pos /= rhs;

        return *this;
    }

    TextureBindedVertex operator+(TextureBindedVertex const& rhs) const
    {
        return TextureBindedVertex{ *this } += rhs;
    }
    TextureBindedVertex operator-(TextureBindedVertex const& rhs) const
    {
        return TextureBindedVertex{ *this } -= rhs;
    }

    TextureBindedVertex operator*(float const& rhs) const
    {
        return TextureBindedVertex{ *this } *= rhs;
    }
    TextureBindedVertex operator/(float const& rhs) const
    {
        return TextureBindedVertex{ *this } /= rhs;
    }

    Vec3 model_pos{ };
    Vec2 texture_pos{ };
};