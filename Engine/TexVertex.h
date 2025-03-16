#pragma once

#include "Vec3.h"


class TexVertex
{
public:

    TexVertex InterpolateTo(TexVertex const& dest, float alpha)
    {
        return
        TexVertex
        {
            model_pos.InterpolateTo(dest.model_pos, alpha),
            texture_pos.InterpolateTo(dest.texture_pos, alpha)
        };
    }

    Vec3 model_pos{ };
    Vec2 texture_pos{ };
};