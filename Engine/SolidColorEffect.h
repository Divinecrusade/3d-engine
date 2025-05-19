#pragma once

#include "Mat3.h"
#include "Colors.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"

#include <utility>

class SolidColorEffectWithVariants
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
        float operator*(ColorBindedVertex const& rhs) const
        {
            return { model_pos * rhs.model_pos };
        }
        ColorBindedVertex operator%(ColorBindedVertex const& rhs) const
        {
            return ColorBindedVertex{ model_pos % rhs.model_pos };
        }

        Vec3 model_pos{ };
        Color color{ };
    };

    using Vertex = Vec3;

    class PixelShader
    {
    public:

        Color operator()(ColorBindedVertex const& v) const
        {
            return v.color;
        }
    };

    class VertexShader
    {
    public:

        using InVertex  = Vertex;
        using OutVertex = InVertex;

        void SaveRotation(Mat3 rot)
        {
            rotation = rotation * rot;
        }

        void SaveTranslation(Vec3 trans)
        {
            translation += trans;
        }

        Mat3 GetRotation() const
        {
            return rotation;
        }

        Vec3 GetTranslation() const
        {
            return translation;
        }

        void SetRotation(Mat3 rot)
        {
            rotation = rot;
        }

        void SetTranslation(Vec3 trans)
        {
            this->translation = trans;
        }

        virtual OutVertex operator()(InVertex const& v) const
        {
            return v * rotation + translation;
        }

    private:

        Mat3 rotation{ Mat3::Identity() };
        Vec3 translation{ 0.f, 0.f, 0.f };
    };
    class GeometryShader
    {
    public:

        using InVertex  = Vertex;
        using OutVertex = ColorBindedVertex;

        GeometryShader(std::vector<Color> colors_pull_init, std::size_t delimeter_init)
        :
        colors_pull{ std::move(colors_pull_init) },
        delimiter{ delimeter_init }
        { }

        std::array<OutVertex, 3ull> operator()(InVertex const& v0, InVertex const& v1, InVertex const& v2, std::size_t i) const
        {
            return std::array<OutVertex, 3ull>
            { 
                OutVertex{ v0, colors_pull[i / delimiter] },
                OutVertex{ v1, colors_pull[i / delimiter] },
                OutVertex{ v2, colors_pull[i / delimiter] }
            };
        }

    private:

        std::vector<Color> colors_pull;
        std::size_t delimiter;
    };

public:

    SolidColorEffectWithVariants(std::vector<Color> colors_pull_init, std::size_t delimeter_init)
    :
    gs{ std::move(colors_pull_init), std::move(delimeter_init) }
    {  }

public:

    PixelShader ps{ };
    VertexShader vs{ };
    GeometryShader gs;
};

class SolidColorEffect
{
public:

    using Vertex = Vec3;

    using VertexShader = DefaultVertexShader<Vertex>;
    class GeometryShader
    {
    public:

        using InVertex = Vertex;
        using OutVertex = SolidColorEffectWithVariants::ColorBindedVertex;

        GeometryShader(Color c)
        : c{ c }
        { }

        std::array<OutVertex, 3ull> operator()(InVertex const& v0, InVertex const& v1, InVertex const& v2, std::size_t) const
        {
            return { OutVertex{ v0, c }, OutVertex{ v1, c }, OutVertex{ v2, c } };
        }

    private:

        Color c;
    };

    using PixelShader = SolidColorEffectWithVariants::PixelShader;

public:

    SolidColorEffect(Color c)
    :
    gs{ c }
    {
    }

public:

    PixelShader ps{ };
    VertexShader vs{ };
    GeometryShader gs;
};