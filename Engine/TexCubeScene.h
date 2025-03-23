#pragma once

#include "Cube.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "PubeScreenTransformer.h"
#include "Mat3.h"
#include "Surface.h"


class TexCubeScene
{
public:

	void Update(Keyboard& kbd, float dt)
	{
		if (kbd.KeyIsPressed('Q'))
		{
			theta_x = wrap_angle(theta_x + dTheta * dt);
		}
		if (kbd.KeyIsPressed('W'))
		{
			theta_y = wrap_angle(theta_y + dTheta * dt);
		}
		if (kbd.KeyIsPressed('E'))
		{
			theta_z = wrap_angle(theta_z + dTheta * dt);
		}
		if (kbd.KeyIsPressed('A'))
		{
			theta_x = wrap_angle(theta_x - dTheta * dt);
		}
		if (kbd.KeyIsPressed('S'))
		{
			theta_y = wrap_angle(theta_y - dTheta * dt);
		}
		if (kbd.KeyIsPressed('D'))
		{
			theta_z = wrap_angle(theta_z - dTheta * dt);
		}
		if (kbd.KeyIsPressed('R'))
		{
			offset_z += 2.0f * dt;
		}
		if (kbd.KeyIsPressed('F'))
		{
			offset_z -= 2.0f * dt;
		}
	}
	void Draw(Graphics& gfx, PubeScreenTransformer const& pst)
	{
		//Vec3 x{ 0.5f, 0.f, 0.f };
		//Vec3 y{ 0.f, 0.5f, 0.f };
		//Vec3 z{ 0.f, 0.f, 0.5f };
		//Vec3 start{ 0.f, 0.f, offset_z };
		auto vertices_to_draw{ cb.GetTrianglesTex() };

		//x *= rot;
		//y *= rot;
		//z *= rot;

		//x += { 0.f, 0.f, offset_z };
		//y += { 0.f, 0.f, offset_z };
		//z += { 0.f, 0.f, offset_z };

		//pst.Transform(x);
		//pst.Transform(y);
		//pst.Transform(z);
		//pst.Transform(start);

		auto const rot
		{
			Mat3::RotationX(theta_x) *
			Mat3::RotationY(theta_y) *
			Mat3::RotationZ(theta_z)
		};

		Vec3 const camera_offset{ 0.0f, 0.0f, offset_z };
		for (auto& vertex : vertices_to_draw.vertices)
		{
			vertex.model_pos *= rot;
			vertex.model_pos += camera_offset;
		}

		for (std::size_t i{ 0U }; i != vertices_to_draw.cullFlags.size(); ++i)
		{
			vertices_to_draw.cullFlags[i] =
				(
					(
						vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u + 1u]].model_pos
						-
						vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u]].model_pos
						)
					%
					(
						vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u + 2u]].model_pos
						-
						vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u]].model_pos
						)
					)
				*
				vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u]].model_pos
		> 0.f;
		}

		for (auto& vertex : vertices_to_draw.vertices)
		{
			pst.Transform(vertex.model_pos);
		}

		for (std::size_t i{ 0U }; i != vertices_to_draw.cullFlags.size(); ++i)
		{
			if (vertices_to_draw.cullFlags[i]) continue;
			gfx.DrawTriangleTex
			(
				vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u]],
				vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u + 1u]],
				vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u + 2u]],
				texture
			);
		}

		//gfx.DrawLine(start, x, Colors::Green);
		//gfx.DrawLine(start, y, Colors::Blue);
		//gfx.DrawLine(start, z, Colors::Red);
	}

private:

	Cube cb{ 1.f };
	Surface texture{ Surface::FromFile(L"Images\\sauron-bhole-100x100.png") };
	static constexpr float dTheta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
};