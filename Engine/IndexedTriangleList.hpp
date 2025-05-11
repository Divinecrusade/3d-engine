#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <string_view>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include "Vec3.h"
#include "tiny_obj_loader.h"
#include "Seb.h"

struct PointAdapterVec3
{
public:

	using T = float;

	float operator[](size_t i) const
	{
		switch (i)
		{
			case 0: return p.x;
			case 1: return p.y;
			case 2: return p.z;
			default: throw std::exception{ "Out of scope of Vec3 dimension" };
		}
	}

	Vec3 const& p;
};

struct PointAccessorAdapterVec3
{
	PointAdapterVec3 const& operator[](size_t i) const
	{
		return PointAdapterVec3{ v.at(i) };
	}

	size_t size() const
	{
		return v.size();
	}

	std::vector<Vec3> const& v;
};


template<typename T = Vec3>
struct IndexedTriangleList
{
	IndexedTriangleList() = default;
    IndexedTriangleList(std::vector<T> init_vertices, std::vector<std::size_t> init_indices)
    :
    vertices{ std::move(init_vertices) },
    indices{ std::move(init_indices) }
    {
        assert(vertices.size() >= 3U);
        assert(indices.size() % 3U == 0U);
    }

    IndexedTriangleList(std::string const& modeldir, std::string const& inputfile)
    {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = modeldir.c_str(); // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(inputfile, reader_config)) {
            if (!reader.Error().empty()) {
                throw std::runtime_error{ reader.Error() };
            }
        }

        if (!reader.Warning().empty()) {
            OutputDebugStringA((std::string{ "TinyObjReader: " } + std::string{ reader.Warning() }).c_str());
        }

        auto& attrib = reader.GetAttrib();

        for (size_t i = 0; i != attrib.vertices.size(); i += 3) {
            tinyobj::real_t vx = attrib.vertices[i + 0];
            tinyobj::real_t vy = attrib.vertices[i + 1];
            tinyobj::real_t vz = attrib.vertices[i + 2];
            vertices.emplace_back(vx, vy, vz);
        }

		auto& shape = reader.GetShapes().front();

		std::transform(shape.mesh.indices.begin(), shape.mesh.indices.end(), std::back_inserter(indices), [](auto const& i)
		{
			return static_cast<std::size_t>(i.vertex_index);
		});
		

        assert(vertices.size() >= 3U);
        assert(indices.size() % 3U == 0U);
    }

	static IndexedTriangleList<T> Load(const std::string& filename)
	{
		IndexedTriangleList<T> tl;

		// check first line of file to see if CCW winding comment exists
		bool isCCW = false;
		{
			std::ifstream file(filename);
			std::string firstline;
			std::getline(file, firstline);
			std::transform(firstline.begin(), firstline.end(), firstline.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });
			if (firstline.find("ccw") != std::string::npos)
			{
				isCCW = true;
			}
		}

		// these will be filled by obj loading function
		using namespace tinyobj;
		attrib_t attrib;
		std::vector<shape_t> shapes;
		std::string err;

		// load/parse the obj file
		const bool ret = LoadObj(&attrib, &shapes, nullptr, &err, &err, filename.c_str());
		//const bool ret = LoadObj(&attrib, &shapes, nullptr, &err, filename.c_str(), &basedir, false, false);

		// check for errors
		if (!err.empty() && err.substr(0, 4) != "WARN")
		{
			throw std::runtime_error(("LoadObj returned error:" + err + " File:" + filename).c_str());
		}
		if (!ret)
		{
			throw std::runtime_error(("LoadObj returned false  File:" + filename).c_str());
		}
		if (shapes.size() == 0u)
		{
			throw std::runtime_error(("LoadObj object file had no shapes  File:" + filename).c_str());
		}

		// extract vertex data
		// attrib.vertices is a flat std::vector of floats corresponding
		// to vertex positions, laid out as xyzxyzxyz... etc.
		// first preallocate required space in OUR std::vector
		tl.vertices.reserve(attrib.vertices.size() / 3u);
		// iterate over individual vertices, construct Vec3s in OUR vector
		for (int i = 0; i < attrib.vertices.size(); i += 3)
		{
			tl.vertices.emplace_back(Vec3{
				attrib.vertices[i + 0],
				attrib.vertices[i + 1],
				attrib.vertices[i + 2]
				});
		}

		// extract index data
		// obj file can contain multiple meshes, we assume just 1
		const auto& mesh = shapes[0].mesh;
		// mesh contains a std::vector of num_face_vertices (uchar)
		// and a flat std::vector of indices. If all faces are triangles
		// then for any face f, the first index of that faces is [f * 3n]
		tl.indices.reserve(mesh.indices.size());
		for (size_t f = 0; f < mesh.num_face_vertices.size(); f++)
		{
			// make sure there are no non-triangle faces
			if (mesh.num_face_vertices[f] != 3u)
			{
				std::stringstream ss;
				ss << "LoadObj error face #" << f << " has "
					<< mesh.num_face_vertices[f] << " vertices";
				throw std::runtime_error(ss.str().c_str());
			}

			// load set of 3 indices for each face into OUR index std::vector
			for (size_t vn = 0; vn < 3u; vn++)
			{
				const auto idx = mesh.indices[f * 3u + vn];
				tl.indices.push_back(size_t(idx.vertex_index));
			}

			// reverse winding if file marked as CCW
			if (isCCW)
			{
				// swapping any two indices reverse the winding dir of triangle
				std::swap(tl.indices.back(), *std::prev(tl.indices.end(), 2));
			}
		}

		return tl;
	}

	static IndexedTriangleList<T> MyLoad(std::string const& filename)
	{
		std::ifstream fin{ filename };
		std::vector<T> vertices{ };
		std::vector<std::size_t> indices{ };

		while (!fin.eof() && fin.good())
		{
			std::string line{ };
			std::getline(fin, line);

			std::istringstream sin{ line };

			if (line.empty()) continue;

			std::string word{ };

			sin >> word;

			char const keychar{ word.front() };

			if (keychar == '#') continue;
			else if (keychar == 'v')
			{
				float x{ };
				float y{ };
				float z{ };

				sin >> x >> y >> z;

				vertices.emplace_back(x, y, z);
			}
			else if (keychar == 'f')
			{
				std::array<std::size_t, 3ull> triangle_indices{ };

				for (auto& i : triangle_indices)
				{
					sin >> i;
					indices.emplace_back(i - 1ull);
				}
			}
		}

		return IndexedTriangleList<T>{ std::move(vertices), std::move(indices) };
	}

	template<class PointAdapter, class PointAccessorAdapter>
	float AdjustFromCenter()
	{
		using namespace SEB_NAMESPACE;

		Smallest_enclosing_ball<PointAdapter::T, PointAdapter, PointAccessorAdapter> sphere{ 3, PointAccessorAdapter{ vertices } };

		auto center{ sphere.center_begin() };
		for (auto& v : vertices)
		{
			v -= T{ center[0], center[1], center[2] };
		}

		return sphere.squared_radius();
	}

	static IndexedTriangleList<T> LoadWithNormals(std::string const& filename)
	{
		std::ifstream fin{ filename };
		std::vector<T> vertices{ };
		std::vector<Vec3> normales{ };
		std::vector<std::size_t> indices{ };

		while (!fin.eof() && fin.good())
		{
			std::string line{ };
			std::getline(fin, line);

			std::istringstream sin{ line };

			if (line.empty()) continue;

			std::string word{ };

			sin >> word;

			if (word.front() == 'f')
			{
				std::array<std::size_t, 3ull> triangle_indices{ };

				for (auto& i : triangle_indices)
				{
					sin >> word;

					//std::size_t pos_sep{ };
					//i = std::stoull(word, &pos_sep) - 1ull;
					//char* end = nullptr;
					//std::size_t j{ std::strtoull(word.c_str() + pos_sep + 2, &end, 10) - 1ull };
					//vertices[i].n = normales[j];
					//indices.push_back(j);
					std::size_t pos_sep{ };
					i = std::stoull(word, &pos_sep) - 1ull;
					char* end = nullptr;
					vertices[i].n = normales[std::strtoull(word.c_str() + pos_sep + 2, &end, 10) - 1ull];
					indices.push_back(i);
				}
			}
			else if (word.front() == 'v')
			{
				float x{ };
				float y{ };
				float z{ };

				sin >> x >> y >> z;

				if (word.size() == 1)
				{
					vertices.emplace_back(Vec3{ x, y, z });
				}
				else if (*std::next(word.begin()) == 'n')
				{
					normales.emplace_back(x, y, z);
				}
			}
		}

		return IndexedTriangleList<T>{ std::move(vertices), std::move(indices) };
	}


    std::vector<T> vertices{ };
    std::vector<std::size_t> indices{ };
};