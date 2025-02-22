#pragma once

#include "Vec3.h"

class PubeScreenTransformer
{
public:

    PubeScreenTransformer(float screen_width, float screen_height)
    :
    screen_half_width{ screen_width / 2.f },
    screen_half_height{ screen_height / 2.f }
    {  }

    Vec3& Transform(Vec3& v)
    {
        v.x = v.x * screen_half_width + screen_half_width;
        v.y = -v.y * screen_half_height + screen_half_height;
    }

    Vec3 GetTransformed(Vec3 const& v)
    {
        return Transform(Vec3{ v });
    }

private:

    float screen_half_width{ };
    float screen_half_height{ };
};