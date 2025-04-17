#pragma once

#include "Mat3.h"

template<class Vertex>
class GeometryShader
{
public:

    using InVertex = Vertex;
    using OutVertex = Vertex;

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

    OutVertex operator()(InVertex const& v) const
    {
        OutVertex vertex{ v };

        vertex.model_pos = vertex.model_pos * rotation + translation;

        return vertex;
    }

private:

    Mat3 rotation{ Mat3::Identity() };
    Vec3 translation{ 0.f, 0.f, 0.f };
};