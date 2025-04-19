#pragma once

#include "ColorBlendEffect.h"
#include <cmath>

class PositionColorEffect
{
public:

    using Vertex = Vec3;

    using PixelShader = ColorBlendEffect::PixelShader;

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
            OutVertex vertex{ v };

            vertex.model_pos = vertex.model_pos * rotation + translation;
            vertex.color = 
                Vec3
                { 
                    std::fabs(vertex.model_pos.x),
                    std::fabs(vertex.model_pos.y),
                    std::fabs(vertex.model_pos.z) 
                } * 255.f;

            return vertex;
        }

    private:

        Mat3 rotation{ Mat3::Identity() };
        Vec3 translation{ 0.f, 0.f, 0.f };
    };

public:

    PixelShader ps{ };
    VertexShader vs{ };
};