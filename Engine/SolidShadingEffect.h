#pragma once

#include "SolidColorEffect.h"
#include "ColorBlendEffect.h"

class SolidShadingEffectV
{
public:

    class VertexWithNormale : public Vec3
    {
    public:

        VertexWithNormale(Vec3 v)
        :
        Vec3{ v }
        { }

        Vec3 GetModel() const
        {
            return Vec3{ x, y, z };
        }

    public:

        Vec3 n{ 0.f, 0.f, 0.f };
    };
    
    using Vertex = VertexWithNormale;

    class PixelShader
    {
    public:

        Color operator()(SolidColorEffect::ColorBindedVertex const& v) const
        {
            return v.color;
        }
    };

    class VertexShader
    {
    public:

        using InVertex = Vertex;
        using OutVertex = SolidColorEffect::ColorBindedVertex;

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
            Vec3 const diffused{ diffuse * std::max(0.f, -(v.n * rotation) * dir) };
            Color const c{ material.GetHadamarded(diffused + ambient).GetSaturated() * 255.f };

            return { v * rotation + translation, c };
        }

        void RotateLight(Mat3 const& rot)
        {
            dir *= rot;
        }

    private:

        Mat3 rotation{ Mat3::Identity() };
        Vec3 translation{ 0.f, 0.f, 0.f };
        
        Vec3 diffuse{ 1.f, 1.f, 1.f };
        Vec3 ambient{ 0.1f, 0.1f, 0.1f };
        Vec3 material{ 1.f, 1.f, 1.f };
        Vec3 dir{ 0.f, 0.f, 1.f };
    };
    using GeometryShader = DefaultGeometryShader<VertexShader::OutVertex>;

public:

    PixelShader ps{ };
    VertexShader vs{ };
    GeometryShader gs{ };
};

class SolidShadingEffectG
{
public:

    using Vertex = Vec3;

    using PixelShader = SolidColorEffect::PixelShader;
    using VertexShader = SolidColorEffect::VertexShader;

    class GeometryShader
    {
    public:

        using InVertex = Vertex;
        using OutVertex = SolidColorEffect::ColorBindedVertex;

        std::array<OutVertex, 3ull> operator()(InVertex const& v0, InVertex const& v1, InVertex const& v2, std::size_t) const
        {
            Vec3 const n{ ((v1 - v0) % (v2 - v0)).GetNormalized() };
            Vec3 const diffused{ diffuse * std::max(0.f, -(n * dir)) };
            Color const c{ material.GetHadamarded(diffused + ambient).GetSaturated() * 255.f };

            return std::array<OutVertex, 3ull>{ OutVertex{ v0, c }, OutVertex{ v1, c }, OutVertex{ v2, c } };
        }

        void RotateLight(Mat3 const& rot)
        {
            dir *= rot;
        }

    private:

        Vec3 diffuse{ 1.f, 1.f, 1.f };
        Vec3 ambient{ 0.3f, 0.3f, 0.3f };
        Vec3 material{ 1.f, 1.0f, 1.0f };
        Vec3 dir{ 0.f, 0.f, 1.f };
    };

public:

    PixelShader ps{ };
    VertexShader vs{ };
    GeometryShader gs{ };
};