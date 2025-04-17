#pragma once

#include "Graphics.h"
#include "IndexedTriangleList.hpp"
#include "Mat3.h"
#include "ChiliMath.h"
#include "ClampEffect.h"

#include <array>
#include <memory>
#include <algorithm>
#include <limits>

template<class GraphicEffect, class Vertex = GraphicEffect::Vertex>
class Pipeline
{
public:

    using Triangle = std::array<Vertex, 3u>;

public:

    Pipeline(Graphics& gfx, GraphicEffect effect)
    :
    gfx{ gfx },
    screen_half_width{ gfx.ScreenWidth / 2.f },
    screen_half_height{ gfx.ScreenHeight / 2.f },
    zbuffer{ std::make_unique<float[]>(gfx.ScreenWidth * gfx.ScreenHeight) },
    effect{ std::move(effect) }
    {
        ResetZBuffer();
    }

    void BeginFrame()
    {
        ResetZBuffer();
    }

    void Draw(IndexedTriangleList<Vertex> model)
    {
        ProcessVertices(model.vertices, model.indices);
    }

    void SaveRotation(Mat3 rot)
    {
        rotation = rotation * rot;
    }

    void SaveTranslation(Vec3 trans)
    {
        translation += trans;
    }

    Mat3 GetRotation() const
    {
        return rotation;
    }

    Vec3 GetTranslation() const
    {
        return translation;
    }

    void SetRotation(Mat3 rot)
    {
        rotation = rot;
    }

    void SetTranslation(Vec3 trans)
    {
        this->translation = trans;
    }

private:

    void ProcessVertices(std::vector<Vertex>& vertices, std::vector<std::size_t> const& indices)
    {
        for (auto& vertex : vertices)
        {
            vertex.model_pos = vertex.model_pos * rotation + translation;
        }
        AssembleTriangles(vertices, indices);
    }

    void AssembleTriangles(std::vector<Vertex>& vertices, std::vector<std::size_t> const& indices)
    {
        for (std::size_t i{ 0u }; i < indices.size(); i += 3u)
        {
            if 
            (
                (vertices[indices[i + 1u]].model_pos - vertices[indices[i]].model_pos)
                %
                (vertices[indices[i + 2u]].model_pos - vertices[indices[i]].model_pos)
                *
                vertices[indices[i]].model_pos > 0.f
            ) continue;

            ProccessTriangle(vertices[indices[i]], vertices[indices[i + 1u]], vertices[indices[i + 2u]]);
        }
    }

    void ProccessTriangle(Vertex& v0, Vertex& v1, Vertex& v2)
    {
        PostProccessTriangle(Triangle{ v0, v1, v2 });
    }

    void PostProccessTriangle(Triangle object)
    {
        for (auto& vertex : object)
        {
            PubeScreenTransform(vertex);
        }
        DrawTriangle(object);
    }

    void PubeScreenTransform(Vertex& v)
    {
        float const zFactor{ 1.f / v.model_pos.z };
        v *= zFactor;
        v.model_pos.x = v.model_pos.x * screen_half_width + screen_half_width;
        v.model_pos.y = -v.model_pos.y * screen_half_height + screen_half_height;
        v.model_pos.z = zFactor;
    }

    void DrawTriangle(Triangle& object)
    {
        Vertex& p0{ object[0u] };
        Vertex& p1{ object[1u] };
        Vertex& p2{ object[2u] };

        if (p0.model_pos.y < p1.model_pos.y) std::swap(p0, p1);
        if (p2.model_pos.y < p1.model_pos.y) std::swap(p2, p1);
        if (p0.model_pos.y > p2.model_pos.y) std::swap(p0, p2);

        if (std::fabs(p0.model_pos.y - p1.model_pos.y) < 1e-7f)
        {
            if (p0.model_pos.x > p1.model_pos.x) std::swap(p0, p1);
            DrawFlatTopTriangle(p2, p0, p1);
        }
        else if (std::fabs(p0.model_pos.y - p2.model_pos.y) < 1e-7f)
        {
            if (p0.model_pos.x > p2.model_pos.x) std::swap(p0, p2);
            DrawFlatBottomTriangle(p1, p0, p2);
        }
        else
        {
            float const alpha{ (p0.model_pos.y - p1.model_pos.y) / (p2.model_pos.y - p1.model_pos.y) };
            Vertex const pi{  interpolate(p1, p2, alpha) };

            if (pi.model_pos.x < p0.model_pos.x)
            {
                DrawFlatBottomTriangle(p1, pi, p0);
                DrawFlatTopTriangle(p2, pi, p0);
            }
            else
            {
                DrawFlatBottomTriangle(p1, p0, pi);
                DrawFlatTopTriangle(p2, p0, pi);
            }
        }
    }

    void DrawFlatTopTriangle(Vertex const& p0, Vertex const& p1, Vertex const& p2)
    {
        float const delta_y{ p0.model_pos.y - p1.model_pos.y };

        Vertex const left_slope_step{ (p0 - p1) / delta_y };
        Vertex const right_slope_step{ (p0 - p2) / delta_y };

        DrawFlatTriangle(p1, p2, left_slope_step, right_slope_step, p0);
    }

    void DrawFlatBottomTriangle(Vertex const& p0, Vertex const& p1, Vertex const& p2)
    {
        float const delta_y{ p1.model_pos.y - p0.model_pos.y };

        Vertex const left_slope_step{ (p1 - p0) / delta_y };
        Vertex const right_slope_step{ (p2 - p0) / delta_y };

        DrawFlatTriangle(p0, p0, left_slope_step, right_slope_step, p1);
    }

    void DrawFlatTriangle(Vertex left_slope, Vertex right_slope, Vertex const& left_slope_step, Vertex const& right_slope_step, Vertex const& to)
    {
        for (float y{ std::ceilf(left_slope.model_pos.y - 0.5f) }; y < std::ceilf(to.model_pos.y - 0.5f); ++y,
            left_slope += left_slope_step, right_slope += right_slope_step)
        {
            auto iLine{ left_slope };

            float const delta_x{ right_slope.model_pos.x - left_slope.model_pos.x };
            auto const step{ (right_slope - left_slope) / delta_x };

            iLine += step * (std::ceilf(left_slope.model_pos.x - 0.5f) + 0.5f - left_slope.model_pos.x);

            for (float x{ std::ceilf(left_slope.model_pos.x - 0.5f) }; x < std::ceilf(right_slope.model_pos.x - 0.5f); ++x,
                iLine += step)
            {
                if (UpdateZBuffer({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) }, 1.f / iLine.model_pos.z))
                {
                    gfx.PutPixel(static_cast<int>(x), static_cast<int>(y), effect.ps(iLine / iLine.model_pos.z));
                }
            }
        }
    }

    void ResetZBuffer()
    {
        std::fill_n(zbuffer.get(), gfx.ScreenWidth * gfx.ScreenHeight, std::numeric_limits<float>::infinity());
    }

    bool UpdateZBuffer(_Vec2<unsigned int> pos, float z)
    {
        if (zbuffer[pos.y * gfx.ScreenWidth + pos.x] > z)
        {
            zbuffer[pos.y * gfx.ScreenWidth + pos.x] = z;
            return true;
        }
        return false;
    }

private:

    Graphics& gfx;
    float screen_half_width;
    float screen_half_height;

    std::unique_ptr<float[]> zbuffer;

    GraphicEffect effect;

    Mat3 rotation{ Mat3::Identity() };
    Vec3 translation{ 0.f, 0.f, 0.f };
};