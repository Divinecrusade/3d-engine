#pragma once

template<class Vertex>
class DefaultGeometryShader
{
public:

    using InVertex  = Vertex;
    using OutVertex = Vertex;

    std::array<OutVertex, 3ull> operator()(InVertex const& v0, InVertex const& v1, InVertex const& v2, std::size_t) const
    {
        return { v0, v1, v2 };
    }
};