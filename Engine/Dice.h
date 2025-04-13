#pragma once

#include "IndexedLineList.h"
#include "IndexedTriangleList.hpp"
#include "TextureBindedVertex.h"


class Dice
{
public:

    static IndexedTriangleList<TextureBindedVertex> GetTrianglesTex(float size)
    {
        std::vector<TextureBindedVertex> object{ };

        auto const half_size{ size / 2.f };
        object.reserve(14u);

        object.emplace_back
        (
            Vec3{ -half_size, -half_size, -half_size },
            Vec2{ 2.f / 3.f, 0.f }
        );

        object.emplace_back
        (
            Vec3{ half_size, -half_size, -half_size },
            Vec2{ 1.f / 3.f, 0.f }
        );

        object.emplace_back
        (
            Vec3{ -half_size, half_size, -half_size },
            Vec2{ 2.f / 3.f, 1.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ half_size, half_size, -half_size },
            Vec2{ 1.f / 3.f, 1.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ -half_size, half_size, half_size },
            Vec2{ 2.f / 3.f, 2.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ half_size, half_size, half_size },
            Vec2{ 1.f / 3.f, 2.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ -half_size, -half_size, -half_size },
            Vec2{ 1.f, 1.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ -half_size, -half_size, half_size },
            Vec2{ 1.f, 2.f / 4.f }
        );

        object.emplace_back
        ( 
            Vec3{ half_size, -half_size, -half_size },
            Vec2{ 0.f, 1.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ half_size, -half_size, half_size },
            Vec2{ 0.f, 2.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ half_size, -half_size, half_size },
            Vec2{ 1.f / 3.f, 3.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ -half_size, -half_size, half_size },
            Vec2{ 2.f / 3.f, 3.f / 4.f }
        );

        object.emplace_back
        (
            Vec3{ -half_size, -half_size, -half_size },
            Vec2{ 2.f / 3.f, 1.f }
        );

        object.emplace_back
        (
            Vec3{ half_size, -half_size, -half_size },
            Vec2{ 1.f / 3.f, 1.f }
        );

        return
        {
            std::move(object),
            {
                1,0,3, 2,3,0,
                3,2,5, 4,5,2,
                2,6,4, 7,4,6,
                8,3,9, 5,9,3,
                5,4,10, 11,10,4,
                10,11,13, 12,13,11
            }
        };
    }
};