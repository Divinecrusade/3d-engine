#pragma once

#include "IndexedLineList.h"
#include "IndexedTriangleList.hpp"


class Cube
{
public:

    Cube(float size)
	{
        auto const half_size{ size / 2.f };

        model.emplace_back(-half_size, -half_size, -half_size);
        model.emplace_back(half_size, -half_size, -half_size);
        model.emplace_back(-half_size, half_size, -half_size);
        model.emplace_back(half_size, half_size, -half_size);

        model.emplace_back(-half_size, -half_size, half_size);
        model.emplace_back(half_size, -half_size, half_size);
        model.emplace_back(-half_size, half_size, half_size);
        model.emplace_back(half_size, half_size, half_size);
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

private:

    std::vector<Vec3> model{ };
};