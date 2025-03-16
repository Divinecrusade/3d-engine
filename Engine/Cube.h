#pragma once

#include "IndexedLineList.h"
#include "IndexedTriangleList.hpp"
#include "TexVertex.h"


class Cube
{
public:

    Cube(float size)
	{
        auto const half_size{ size / 2.f };
        model.reserve(8u);
        texture.reserve(8u);

        model.emplace_back(-half_size, -half_size, -half_size);
        texture.emplace_back(0.f, 1.f);
        model.emplace_back(half_size, -half_size, -half_size);
        texture.emplace_back(1.f, 1.f);
        model.emplace_back(-half_size, half_size, -half_size);
        texture.emplace_back(0.f, 0.f);
        model.emplace_back(half_size, half_size, -half_size);
        texture.emplace_back(1.f, 0.f);

        model.emplace_back(-half_size, -half_size, half_size);
        texture.emplace_back(1.f, 1.f);
        model.emplace_back(half_size, -half_size, half_size);
        texture.emplace_back(0.f, 1.f);
        model.emplace_back(-half_size, half_size, half_size);
        texture.emplace_back(1.f, 0.f);
        model.emplace_back(half_size, half_size, half_size);
        texture.emplace_back(0.f, 0.f);
	}

	IndexedLineList GetLines() const
	{
        return IndexedLineList
        {
            model,
            { 0U,1U, 0U,2U, 0U,4U, 1U,3U, 1U,5U, 2U,3U, 2U,6U, 4U,6U, 4U,5U, 3U,7U, 7U,6U, 7U,5U }
		};
	}

    IndexedTriangleList<Vec3> GetTriangles() const
    {
        return 
        {
            model,
            {
                0,2,1, 2,3,1, 
                1,3,5, 3,7,5, 
                0,4,2, 2,4,6,
                2,6,3, 3,6,7,
                4,5,7, 4,7,6,
                0,1,4, 1,5,4
            }
        };
    }

    IndexedTriangleList<TexVertex> GetTrianglesTex() const
    {
        std::vector<TexVertex> tc{ };
        tc.reserve(model.size());
        std::generate(tc.begin(), tc.end(), [it1 = this->model.cbegin(), it2 = this->texture.cbegin()]() mutable { return TexVertex{ *(it1++), *(it2++) }; });
        return
        {
            std::move(tc),
            {
                0,2,1, 2,3,1,
                1,3,5, 3,7,5,
                0,4,2, 2,4,6,
                2,6,3, 3,6,7,
                4,5,7, 4,7,6,
                0,1,4, 1,5,4
            }
        };
    }

private:

    std::vector<Vec3> model{ };
    std::vector<Vec2> texture{ };
};