#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Colors.h"
#include "DefaultVertexShader.h"

#include <utility>

class ColorBlendEffect
{
public:

    class ColorBindedVertex
    {
    public:

        ColorBindedVertex(Vec3 model, Vec3 color = {0.f, 0.f, 0.f})
            :
            model_pos{ std::move(model) },
            color{ std::move(color) }
        {
        }

        ColorBindedVertex& operator+=(ColorBindedVertex const& rhs)
        {
            model_pos += rhs.model_pos;
            color += rhs.color;

            return *this;
        }
        ColorBindedVertex& operator-=(ColorBindedVertex const& rhs)
        {
            model_pos -= rhs.model_pos;
            color -= rhs.color;

            return *this;
        }

        ColorBindedVertex& operator*=(float const& rhs)
        {
            model_pos *= rhs;
            color *= rhs;

            return *this;
        }
        ColorBindedVertex& operator/=(float const& rhs)
        {
            model_pos /= rhs;
            color /= rhs;

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
        Vec3 color{ };
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

    using VertexShader = DefaultVertexShader<Vertex>;

public:

    PixelShader ps{ };
    VertexShader vs{ };
};