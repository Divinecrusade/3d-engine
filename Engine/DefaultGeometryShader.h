#pragma once

template<class Vertex>
class DefaultGeometryShader
{
public:

    using InVertex  = Vertex;
    using OutVertex = Vertex;

    OutVertex operator()(InVertex const& v, std::size_t) const
    {
        return v;
    }
};