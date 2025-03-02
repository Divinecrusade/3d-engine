#pragma once

#include "IndexedLineList.h"
#include "IndexedTriangleList.hpp"


class Cube
{
public:

    Cube(float size)
	{
        auto const half_size{ size / 2.f };

        model.emplace_back(-half_size, half_size, half_size);
        model.emplace_back(half_size, half_size, half_size);
        model.emplace_back(-half_size, half_size, -half_size);
        model.emplace_back(half_size, half_size, -half_size);

        model.emplace_back(-half_size, -half_size, half_size);
        model.emplace_back(half_size, -half_size, half_size);
        model.emplace_back(-half_size, -half_size, -half_size);
        model.emplace_back(half_size, -half_size, -half_size);
	}

	IndexedLineList GetLines() const
	{
        return IndexedLineList
        {
            model,
            { 0U,1U, 0U,2U, 0U,4U, 1U,3U, 1U,5U, 2U,3U, 2U,6U, 4U,6U, 4U,5U, 3U,7U, 7U,6U, 7U,5U }
		};
	}

    IndexedTriangleList GetTriangles() const
    {
        return IndexedTriangleList
        {
            model,
            {
                0u,1u,5u, 0u,4u,5u, 
                0u,2u,6u, 0u,4u,6u, 
                2u,3u,7u, 2u,6u,7u,
                1u,3u,5u, 3u,7u,5u,
                2u,0u,1u, 2u,3u,1u,
                6u,7u,4u, 4u,5u,7u
            }
        };
    }

private:

    std::vector<Vec3> model{ };
};