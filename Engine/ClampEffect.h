#pragma once

#include "Surface.h"
#include "TextureBindedVertex.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"

#include <algorithm>


class ClampEffect
{
public:

    using Vertex = TextureBindedVertex;

    class PixelShader
    {
    public:

        PixelShader(Surface texture)
        :
        texture{ std::move(texture) }
        { }

        Color operator()(Vertex const& v) const
        {
            
            return texture.GetPixel
            (
                std::min(v.texture_pos.x * texture.GetWidth(), static_cast<float>(texture.GetWidth() - 1)),
                std::min(v.texture_pos.y * texture.GetHeight(), static_cast<float>(texture.GetHeight() - 1))
            );
        }

    private:

        Surface texture;
    };

    using VertexShader = DefaultVertexShader<Vertex>;
    using GeometryShader = DefaultGeometryShader<VertexShader::OutVertex>;

public:

    ClampEffect(Surface texture)
    :
    ps{ std::move(texture) }
    { }
   
public:

    PixelShader ps;
    VertexShader vs{ };
    GeometryShader gs{ };
};