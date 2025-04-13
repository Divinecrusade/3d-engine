#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Colors.h"

#include <utility>

class SolidColorEffect
{
public:

    class ColorBindedVertex
    {
    public:

        ColorBindedVertex(Vec3 model, Color color = { 0, 0, 0 })
            :
            model_pos{ std::move(model) },
            color{ std::move(color) }
        {
        }

        ColorBindedVertex& operator+=(ColorBindedVertex const& rhs)
        {
            model_pos += rhs.model_pos;

            return *this;
        }
        ColorBindedVertex& operator-=(ColorBindedVertex const& rhs)
        {
            model_pos -= rhs.model_pos;

            return *this;
        }

        ColorBindedVertex& operator*=(float const& rhs)
        {
            model_pos *= rhs;

            return *this;
        }
        ColorBindedVertex& operator/=(float const& rhs)
        {
            model_pos /= rhs;

            return *this;
        }

        ColorBindedVertex operator+(ColorBindedVertex const& rhs) const
        {
            return ColorBindedVertex{ *this } += rhs;
        }
        ColorBindedVertex operator-(ColorBindedVertex const& rhs) const
        {
            return ColorBindedVertex{ *this } -= rhs;
        }

        ColorBindedVertex operator*(float const& rhs) const
        {
            return ColorBindedVertex{ *this } *= rhs;
        }
        ColorBindedVertex operator/(float const& rhs) const
        {
            return ColorBindedVertex{ *this } /= rhs;
        }

        Vec3 model_pos{ };
        Color color{ };
    };

    using Vertex = ColorBindedVertex;

    class PixelShader
    {
    public:

        Color operator()(Vertex const& v) const
        {
            return v.color;
        }
    };

public:

    PixelShader ps{ };
};