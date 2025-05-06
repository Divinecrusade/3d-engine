#pragma once

#include "IndexedTriangleList.hpp"
#include "Mat3.h"


class Sphere
{
public:
    
	//template<class V>
	//static IndexedTriangleList<V> GetTriangles(float radius, int latDiv = 6, int longDiv = 12)
 //   {
	//	const Vec3 base = { 0.0f,0.0f,radius/2.f };
	//	const float lattitudeAngle = PI / latDiv;
	//	const float longitudeAngle = 2.0f * PI / longDiv;

	//	std::vector<V> vertices;
	//	for (int iLat = 1; iLat < latDiv; iLat++)
	//	{
	//		const auto latBase = base * Mat3::RotationX(lattitudeAngle * iLat);
	//		for (int iLong = 0; iLong < longDiv; iLong++)
	//		{
	//			vertices.emplace_back();
	//			vertices.back() = latBase * Mat3::RotationZ(longitudeAngle * iLong);
	//		}
	//	}

	//	// add the cap vertices
	//	const auto iNorthPole = vertices.size();
	//	vertices.emplace_back();
	//	vertices.back() = base;
	//	const auto iSouthPole = vertices.size();
	//	vertices.emplace_back();
	//	vertices.back() = -base;

	//	const auto calcIdx = [latDiv, longDiv](int iLat, int iLong)
	//		{ return iLat * longDiv + iLong; };
	//	std::vector<size_t> indices;
	//	for (int iLat = 0; iLat < latDiv - 2; iLat++)
	//	{
	//		for (int iLong = 0; iLong < longDiv - 1; iLong++)
	//		{
	//			indices.push_back(calcIdx(iLat, iLong));
	//			indices.push_back(calcIdx(iLat + 1, iLong));
	//			indices.push_back(calcIdx(iLat, iLong + 1));
	//			indices.push_back(calcIdx(iLat, iLong + 1));
	//			indices.push_back(calcIdx(iLat + 1, iLong));
	//			indices.push_back(calcIdx(iLat + 1, iLong + 1));
	//		}
	//		// wrap band
	//		indices.push_back(calcIdx(iLat, longDiv - 1));
	//		indices.push_back(calcIdx(iLat + 1, longDiv - 1));
	//		indices.push_back(calcIdx(iLat, 0));
	//		indices.push_back(calcIdx(iLat, 0));
	//		indices.push_back(calcIdx(iLat + 1, longDiv - 1));
	//		indices.push_back(calcIdx(iLat + 1, 0));
	//	}

	//	// cap fans
	//	for (int iLong = 0; iLong < longDiv - 1; iLong++)
	//	{
	//		// north
	//		indices.push_back(iNorthPole);
	//		indices.push_back(calcIdx(0, iLong));
	//		indices.push_back(calcIdx(0, iLong + 1));
	//		// south
	//		indices.push_back(calcIdx(latDiv - 2, iLong + 1));
	//		indices.push_back(calcIdx(latDiv - 2, iLong));
	//		indices.push_back(iSouthPole);
	//	}
	//	// wrap triangles
	//	// north
	//	indices.push_back(iNorthPole);
	//	indices.push_back(calcIdx(0, longDiv - 1));
	//	indices.push_back(calcIdx(0, 0));
	//	// south
	//	indices.push_back(calcIdx(latDiv - 2, 0));
	//	indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
	//	indices.push_back(iSouthPole);


	//	return{ std::move(vertices),std::move(indices) };
	//}

	template<class Vertex>
	static IndexedTriangleList<Vertex> GetTriangles(float size, int latDiv = 6, int longDiv = 12)
	{
        Vec3 base{0.f, 0.f, size / 2.f};
        float const lat_delta_angle{ PI / latDiv };
        float const long_delta_angle{ 2.f * PI / longDiv };

        std::vector<Vertex> model{ };

        for (int latitude{ 1 }; latitude < latDiv; ++latitude)
        {
            Vec3 latt_base{ base * Mat3::RotationX(latitude * lat_delta_angle) };
            for (int longitude{ 0 }; longitude < longDiv; ++longitude)
            {
                model.emplace_back(latt_base * Mat3::RotationZ(longitude * long_delta_angle));
            }
        }

        std::vector<std::size_t> indices{ };

        auto north_i{ model.size() };
        model.emplace_back(Vec3{ base });

        auto south_i{ model.size() };
        model.emplace_back(Vec3{ -base });

        auto const calc_i
        {
            [longDiv](int latitude, int longitude)
            {
                return static_cast<std::size_t>(longDiv * latitude + longitude);
            }
        };
        for (int latitude{ 0 }; latitude < latDiv - 2; ++latitude)
        {
            for (int longitude{ 0 }; longitude < longDiv - 1; ++longitude)
            {
                indices.emplace_back(calc_i(latitude, longitude));
                indices.emplace_back(calc_i(latitude + 1, longitude));
                indices.emplace_back(calc_i(latitude, longitude + 1));

                indices.emplace_back(calc_i(latitude, longitude + 1));
                indices.emplace_back(calc_i(latitude + 1, longitude));
                indices.emplace_back(calc_i(latitude + 1, longitude + 1));
            }

            indices.emplace_back(calc_i(latitude, longDiv - 1));
            indices.emplace_back(calc_i(latitude + 1, longDiv - 1));
            indices.emplace_back(calc_i(latitude, 0));

            indices.emplace_back(calc_i(latitude, 0));
            indices.emplace_back(calc_i(latitude + 1, longDiv - 1));
            indices.emplace_back(calc_i(latitude + 1, 0));
        }

        for (int longitude{0}; longitude < longDiv - 1; ++longitude)
        {
            indices.emplace_back(north_i);
            indices.emplace_back(calc_i(0, longitude));
            indices.emplace_back(calc_i(0, longitude + 1));

            indices.emplace_back(calc_i(latDiv - 2, longitude + 1));
            indices.emplace_back(calc_i(latDiv - 2, longitude));
            indices.emplace_back(south_i);
        }
        indices.emplace_back(north_i);
        indices.emplace_back(calc_i(0, longDiv - 1));
        indices.emplace_back(calc_i(0, 0));

        indices.emplace_back(south_i);
        indices.emplace_back(calc_i(latDiv - 2, 0));
        indices.emplace_back(calc_i(latDiv - 2, longDiv - 1));

        return IndexedTriangleList<Vertex>{ std::move(model), std::move(indices) };
    }
};