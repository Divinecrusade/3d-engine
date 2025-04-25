#pragma once

#include "IndexedLineList.h"
#include "IndexedTriangleList.hpp"
#include "TextureBindedVertex.h"


class Cube
{
public:

    template<class Vertex>
    static IndexedTriangleList<Vertex> GetPlains(float size)
    {
        std::vector<Vertex> model{ };

        auto const half_size{ size / 2.f };
        model.reserve(8u);

        model.emplace_back(Vec3{ -half_size, -half_size, -half_size });
        model.emplace_back(Vec3{ half_size, -half_size, -half_size });
        model.emplace_back(Vec3{ -half_size, half_size, -half_size });
        model.emplace_back(Vec3{ half_size, half_size, -half_size });

        model.emplace_back(Vec3{ -half_size, -half_size, half_size });
        model.emplace_back(Vec3{ half_size, -half_size, half_size });
        model.emplace_back(Vec3{ -half_size, half_size, half_size });
        model.emplace_back(Vec3{ half_size, half_size, half_size });

        return
        IndexedTriangleList<Vertex>{
            std::move(model),
            std::vector<std::size_t>{
                0,2,1, 2,3,1,
                1,3,5, 3,7,5,
                0,4,2, 2,4,6,
                2,6,3, 3,6,7,
                4,5,7, 4,7,6,
                0,1,4, 1,5,4
            }
        };
    }

    template<class Vertex>
    static IndexedTriangleList<Vertex> GetTriangles(float size)
    {
        std::vector<Vertex> object{ };

        auto const half_size{ size / 2.f };
        object.reserve(24u);

        object.emplace_back(Vec3{ -half_size, half_size, -half_size });
        object.emplace_back(Vec3{ half_size, half_size, -half_size });
        object.emplace_back(Vec3{ half_size, -half_size, -half_size });
        object.emplace_back(Vec3{ -half_size, -half_size, -half_size });

        object.emplace_back(Vec3{ half_size, half_size, -half_size });
        object.emplace_back(Vec3{ half_size, half_size, half_size });
        object.emplace_back(Vec3{ half_size, -half_size, half_size });
        object.emplace_back(Vec3{ half_size, -half_size, -half_size });

        object.emplace_back(Vec3{ half_size, half_size, half_size });
        object.emplace_back(Vec3{ -half_size, half_size, half_size });
        object.emplace_back(Vec3{ -half_size, -half_size, half_size });
        object.emplace_back(Vec3{ half_size, -half_size, half_size });

        object.emplace_back(Vec3{ -half_size, half_size, half_size });
        object.emplace_back(Vec3{ -half_size, half_size, -half_size });
        object.emplace_back(Vec3{ -half_size, -half_size, -half_size });
        object.emplace_back(Vec3{ -half_size, -half_size, half_size });

        object.emplace_back(Vec3{ -half_size, half_size, half_size });
        object.emplace_back(Vec3{ half_size, half_size, half_size });
        object.emplace_back(Vec3{ half_size, half_size, -half_size });
        object.emplace_back(Vec3{ -half_size, half_size, -half_size });

        object.emplace_back(Vec3{ -half_size, -half_size, -half_size });
        object.emplace_back(Vec3{ half_size, -half_size, -half_size });
        object.emplace_back(Vec3{ half_size, -half_size, half_size });
        object.emplace_back(Vec3{ -half_size, -half_size, half_size });

        return
        {
            std::move(object),
            std::vector<std::size_t>{
                0,1,3, 1,2,3,
                4,5,7, 5,6,7,
                8,9,11, 9,10,11,
                12,13,15, 13,14,15,
                16,17,19, 17,18,19,
                20,21,23, 21,22,23
            }
        };
    }

    template<class Vertex>
    static IndexedTriangleList<Vertex> GetTrianglesWithNormals(float size)
    {
        auto mesh{ GetTriangles<Vertex>(size) };

        mesh.vertices[0].n = Vec3{ 0.f, 0.f, -1.f };
        mesh.vertices[1].n = Vec3{ 0.f, 0.f, -1.f };
        mesh.vertices[2].n = Vec3{ 0.f, 0.f, -1.f };
        mesh.vertices[3].n = Vec3{ 0.f, 0.f, -1.f };

        mesh.vertices[4].n = Vec3{ 1.f, 0.f, 0.f };
        mesh.vertices[5].n = Vec3{ 1.f, 0.f, 0.f };
        mesh.vertices[6].n = Vec3{ 1.f, 0.f, 0.f };
        mesh.vertices[7].n = Vec3{ 1.f, 0.f, 0.f };

        mesh.vertices[8].n = Vec3{ 0.f, 0.f, 1.f };
        mesh.vertices[9].n = Vec3{ 0.f, 0.f, 1.f };
        mesh.vertices[10].n = Vec3{ 0.f, 0.f, 1.f };
        mesh.vertices[11].n = Vec3{ 0.f, 0.f, 1.f };

        mesh.vertices[12].n = Vec3{ -1.f, 0.f, 0.f };
        mesh.vertices[13].n = Vec3{ -1.f, 0.f, 0.f };
        mesh.vertices[14].n = Vec3{ -1.f, 0.f, 0.f };
        mesh.vertices[15].n = Vec3{ -1.f, 0.f, 0.f };

        mesh.vertices[16].n = Vec3{ 0.f, 1.f, 0.f };
        mesh.vertices[17].n = Vec3{ 0.f, 1.f, 0.f };
        mesh.vertices[18].n = Vec3{ 0.f, 1.f, 0.f };
        mesh.vertices[19].n = Vec3{ 0.f, 1.f, 0.f };

        mesh.vertices[20].n = Vec3{ 0.f, -1.f, -1.f };
        mesh.vertices[21].n = Vec3{ 0.f, -1.f, -1.f };
        mesh.vertices[22].n = Vec3{ 0.f, -1.f, -1.f };
        mesh.vertices[23].n = Vec3{ 0.f, -1.f, -1.f };

        return mesh;
    }

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