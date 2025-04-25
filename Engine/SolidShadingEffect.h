#pragma once

#include "SolidColorEffect.h"
#include "ColorBlendEffect.h"

class SolidShadingEffect
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