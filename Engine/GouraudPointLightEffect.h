#pragma once

#include "GouraudShadingEffect.h"

class GourandPointLightEffect
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
            auto transformed_v{ v * rotation + translation };

            auto const to_light{ light_pos - transformed_v };
            auto const distance_to_light{ to_light.Len() };
            auto const to_light_n{ to_light / distance_to_light };
            auto const attenuation{ 1.f / 
            (quadradic_attenuation * distance_to_light * distance_to_light + linear_attenuation * distance_to_light + constant_attenuation) };

            Vec3 const diffused
            { 
                diffuse * attenuation* std::max(0.f, (v.n * rotation) * to_light_n) 
            };
            Color const c{ material.GetHadamarded(diffused + ambient).GetSaturated() * 255.f };

            return { transformed_v, c };
        }

        void MoveLight(Vec3 delta_pos)
        {
            light_pos += delta_pos;
        }

    private:

        Mat3 rotation{ Mat3::Identity() };
        Vec3 translation{ 0.f, 0.f, 0.f };

        Vec3 diffuse{ 1.f, 1.f, 1.f };
        Vec3 ambient{ 0.1f, 0.1f, 0.1f };
        Vec3 material{ 1.f, 1.f, 1.f };

        float linear_attenuation{ 1.6f };
        float quadradic_attenuation{ 1.f };
        float constant_attenuation{ 0.3f };

        Vec3 light_pos{ 0.f, 0.f, 0.f };
    };
    using GeometryShader = GourandShadingEffect::GeometryShader;
    using PixelShader = GourandShadingEffect::PixelShader;

public:

    PixelShader ps{ };
    VertexShader vs{ };
    GeometryShader gs{ };
};