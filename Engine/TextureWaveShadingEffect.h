#pragma once

#include "ColorBlendEffect.h"
#include "SolidColorEffect.h"
#include "ClampEffect.h"
#include "WaveEffect.h"

class TextureWaveShadingEffect
{
public:

    using Vertex = TextureBindedVertex;

    using VertexShader = WaveEffect::VertexShader;

    class TextureLightBindedVector : public TextureBindedVertex
    {
    public:

        TextureLightBindedVector(TextureBindedVertex v, float light_component_init)
        :
        TextureBindedVertex{ v },
        light_component{ light_component_init }
        { }

        TextureLightBindedVector& operator+=(TextureLightBindedVector const& rhs)
        {
            model_pos += rhs.model_pos;
            texture_pos += rhs.texture_pos;

            return *this;
        }
        TextureLightBindedVector& operator-=(TextureLightBindedVector const& rhs)
        {
            model_pos -= rhs.model_pos;
            texture_pos -= rhs.texture_pos;

            return *this;
        }

        TextureLightBindedVector& operator*=(float const& rhs)
        {
            model_pos *= rhs;
            texture_pos *= rhs;

            return *this;
        }
        TextureLightBindedVector& operator/=(float const& rhs)
        {
            model_pos /= rhs;
            texture_pos /= rhs;

            return *this;
        }

        TextureLightBindedVector operator+(TextureLightBindedVector const& rhs) const
        {
            return TextureLightBindedVector{ *this } += rhs;
        }
        TextureLightBindedVector operator-(TextureLightBindedVector const& rhs) const
        {
            return TextureLightBindedVector{ *this } -= rhs;
        }

        TextureLightBindedVector operator*(float const& rhs) const
        {
            return TextureLightBindedVector{ *this } *= rhs;
        }
        TextureLightBindedVector operator/(float const& rhs) const
        {
            return TextureLightBindedVector{ *this } /= rhs;
        }
        float operator*(TextureLightBindedVector const& rhs) const
        {
            return this->model_pos * rhs.model_pos;
        }

    public:

        float light_component;
    };

    class GeometryShader
    {
    public:

        using InVertex = Vertex;
        using OutVertex = TextureLightBindedVector;

        std::array<OutVertex, 3ull> operator()(InVertex const& v0, InVertex const& v1, InVertex const& v2, std::size_t) const
        {
            Vec3 const n{ ((v1.model_pos - v0.model_pos) % (v2.model_pos - v0.model_pos)).GetNormalized() };
            float const l{ (std::min(1.f, diffuse * std::max(0.f, -(n * dir)) + ambient)) };

            return std::array<OutVertex, 3ull>{ OutVertex{ v0, l }, OutVertex{ v1, l }, OutVertex{ v2, l } };
        }

        void RotateLight(Mat3 const& rot)
        {
            dir *= rot;
        }

    private:

        float diffuse{ 1.f };
        float ambient{ 0.1f };
        Vec3 dir{ 0.f, 0.f, 1.f };
    };

    class PixelShader
    {
    public:

        PixelShader(Surface texture)
        :
        texture{ std::move(texture) }
        { }

        Color operator()(GeometryShader::OutVertex const& v) const
        {
            auto const material
            {
                (Vec3)texture.GetPixel
                (
                    std::min(v.texture_pos.x * texture.GetWidth(), static_cast<float>(texture.GetWidth() - 1)),
                    std::min(v.texture_pos.y * texture.GetHeight(), static_cast<float>(texture.GetHeight() - 1))
                )
            };
            return material * v.light_component;
        }

    private:

        Surface texture;
    };

public:

    TextureWaveShadingEffect(Surface texture)
    :
    ps{ std::move(texture) }
    { }

public:

    PixelShader ps;
    VertexShader vs{ };
    GeometryShader gs{ };
};