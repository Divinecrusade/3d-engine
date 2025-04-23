#pragma once

#include "Graphics.h"
#include "IndexedTriangleList.hpp"
#include "ChiliMath.h"
#include "ClampEffect.h"

#include <array>
#include <memory>
#include <algorithm>
#include <limits>

template<class GraphicEffect, 
         class Vertex = GraphicEffect::Vertex, 
         class VSV = GraphicEffect::VertexShader::OutVertex,
         class GSV = GraphicEffect::GeometryShader::OutVertex>
class Pipeline
{
public:

    using Triangle = std::array<GSV, 3u>;

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

private:

    void ProcessVertices(std::vector<Vertex> const& vertices, std::vector<std::size_t> const& indices)
    {
        std::vector<VSV> processed{ };

        std::transform(vertices.begin(), vertices.end(), std::back_inserter(processed), effect.vs);

        AssembleTriangles(processed, indices);
    }

    void AssembleTriangles(std::vector<VSV>& vertices, std::vector<std::size_t> const& indices)
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

            ProccessTriangle(Triangle{ vertices[indices[i]], vertices[indices[i + 1u]], vertices[indices[i + 2u]] }, i / 3ull);
        }
    }

    void ProccessTriangle(Triangle object, std::size_t i)
    {
        for (auto& v : object)
        {
            v = effect.gs(v, i);
        }
        PostProccessTriangle(std::move(object));
    }

    void PostProccessTriangle(Triangle object)
    {
        for (auto& vertex : object)
        {
            PubeScreenTransform(vertex);
        }
        DrawTriangle(object);
    }

    void PubeScreenTransform(VSV& v)
    {
        float const zFactor{ 1.f / v.model_pos.z };
        v *= zFactor;
        v.model_pos.x = v.model_pos.x * screen_half_width + screen_half_width;
        v.model_pos.y = -v.model_pos.y * screen_half_height + screen_half_height;
        v.model_pos.z = zFactor;
    }

    void DrawTriangle(Triangle& object)
    {
        auto& p0{ object[0u] };
        auto& p1{ object[1u] };
        auto& p2{ object[2u] };

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
            auto const pi{  interpolate(p1, p2, alpha) };

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

    void DrawFlatTopTriangle(VSV const& p0, VSV const& p1, VSV const& p2)
    {
        float const delta_y{ p0.model_pos.y - p1.model_pos.y };

        auto const left_slope_step{ (p0 - p1) / delta_y };
        auto const right_slope_step{ (p0 - p2) / delta_y };

        DrawFlatTriangle(p1, p2, left_slope_step, right_slope_step, p0);
    }

    void DrawFlatBottomTriangle(VSV const& p0, VSV const& p1, VSV const& p2)
    {
        float const delta_y{ p1.model_pos.y - p0.model_pos.y };

        auto const left_slope_step{ (p1 - p0) / delta_y };
        auto const right_slope_step{ (p2 - p0) / delta_y };

        DrawFlatTriangle(p0, p0, left_slope_step, right_slope_step, p1);
    }

    void DrawFlatTriangle(VSV left_slope, VSV right_slope, VSV const& left_slope_step, VSV const& right_slope_step, VSV const& to)
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

public:

    GraphicEffect effect;
};