#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Colors.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include "SolidShadingEffect.h"

#include <utility>

class GourandShadingEffect
{
public:

    using Vertex = SolidShadingEffectV::VertexWithNormale;

    class VertexShader
    {
    public:

        using InVertex = Vertex;
        using OutVertex = ColorBlendEffect::ColorBindedVertex;

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
    class PixelShader
    {
    public:

        Color operator()(GeometryShader::OutVertex const& v) const
        {
            return v.color;
        }
    };

public:

    PixelShader ps{ };
    VertexShader vs{ };
    GeometryShader gs{ };
};