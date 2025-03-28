#pragma once

#include "IndexedLineList.h"
#include "IndexedTriangleList.hpp"
#include "TexVertex.h"


class CubeExtended
{
public:

    CubeExtended(float size)
    {
        auto const half_size{ size / 2.f };
        model.reserve(8u);
        texture.reserve(8u);

        model.emplace_back(-half_size, -half_size, -half_size); // 0
        texture.emplace_back(1.f, 0.f);

        model.emplace_back(half_size, -half_size, -half_size); // 1
        texture.emplace_back(2.f, 0.f);

        model.emplace_back(-half_size, half_size, -half_size); // 2
        texture.emplace_back(1.f, 1.f);

        model.emplace_back(half_size, half_size, -half_size); // 3
        texture.emplace_back(2.f, 1.f);

        model.emplace_back(-half_size, half_size, half_size); // 4
        texture.emplace_back(1.f, 2.f);

        model.emplace_back(half_size, half_size, half_size); // 5
        texture.emplace_back(2.f, 2.f);

        model.emplace_back(-half_size, -half_size, -half_size); // 6
        texture.emplace_back(0.f, 1.f);

        model.emplace_back(-half_size, -half_size, half_size); // 7
        texture.emplace_back(0.f, 2.f);

        model.emplace_back(half_size, -half_size, -half_size); // 8
        texture.emplace_back(3.f, 1.f);

        model.emplace_back(half_size, -half_size, half_size); // 9
        texture.emplace_back(3.f, 2.f);

        model.emplace_back(half_size, -half_size, half_size); // 10
        texture.emplace_back(2.f, 3.f);

        model.emplace_back(-half_size, -half_size, half_size); // 11
        texture.emplace_back(1.f, 3.f);

        model.emplace_back(-half_size, -half_size, -half_size); // 12
        texture.emplace_back(1.f, 4.f);

        model.emplace_back(half_size, -half_size, -half_size); // 13
        texture.emplace_back(2.f, 4.f);
    }

    IndexedTriangleList<TexVertex> GetTrianglesTex() const
    {
        std::vector<TexVertex> tc{ };
        tc.reserve(model.size());
        std::generate_n(std::back_inserter(tc), model.size(), [it1 = this->model.cbegin(), it2 = this->texture.cbegin()]() mutable { return TexVertex{ *(it1++), *(it2++) }; });
        return
        {
            std::move(tc),
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

private:

    std::vector<Vec3> model{ };
    std::vector<Vec2> texture{ };
};