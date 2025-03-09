/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Mat3.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 1.0f / 60.0f;
	if( wnd.kbd.KeyIsPressed( 'Q' ) )
	{
		theta_x = wrap_angle( theta_x + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'W' ) )
	{
		theta_y = wrap_angle( theta_y + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'E' ) )
	{
		theta_z = wrap_angle( theta_z + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'A' ) )
	{
		theta_x = wrap_angle( theta_x - dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'S' ) )
	{
		theta_y = wrap_angle( theta_y - dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'D' ) )
	{
		theta_z = wrap_angle( theta_z - dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'R' ) )
	{
		offset_z += 2.0f * dt;
	}
	if( wnd.kbd.KeyIsPressed( 'F' ) )
	{
		offset_z -= 2.0f * dt;
	}
}

void Game::ComposeFrame()
{
	//Vec3 x{ 0.5f, 0.f, 0.f };
	//Vec3 y{ 0.f, 0.5f, 0.f };
	//Vec3 z{ 0.f, 0.f, 0.5f };
	//Vec3 start{ 0.f, 0.f, offset_z };
	auto vertices_to_draw{ cb.GetTriangles() };

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
		vertex *= rot;
		vertex += camera_offset;
	}

	for (std::size_t i{ 0U }; i != vertices_to_draw.cullFlags.size(); ++i)
	{
		vertices_to_draw.cullFlags[i] = 
		(
			(
				vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u + 1u]] 
				- 
				vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u]]
			) 
			% 
			(
				vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u + 2u]] 
				- 
				vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u]]
			)
		)
		*
		vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u]]
		> 0.f;
	}

	for (auto& vertex : vertices_to_draw.vertices)
	{
		pst.Transform(vertex);
	}

	constexpr Color c_pull[]
	{
		Colors::White,
		Colors::Yellow,
		Colors::Red,
		Colors::Green,
		Colors::Blue,
		Colors::MakeRGB(255, 255, 0),
		Colors::MakeRGB(0, 255, 255),
		Colors::MakeRGB(255, 0, 255),
		Colors::MakeRGB(127, 127, 0),
		Colors::MakeRGB(127, 127, 127),
		Colors::MakeRGB(127, 0, 127),
		Colors::MakeRGB(0, 0, 127)
	};
	for (std::size_t i{ 0U }; i != vertices_to_draw.cullFlags.size(); ++i)
	{
		if (vertices_to_draw.cullFlags[i]) continue;
		gfx.DrawTriangle
		(
			vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u]], 
			vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u + 1u]],
			vertices_to_draw.vertices[vertices_to_draw.indices[i * 3u + 2u]], 
			c_pull[i]
		);
	}

	//gfx.DrawLine(start, x, Colors::Green);
	//gfx.DrawLine(start, y, Colors::Blue);
	//gfx.DrawLine(start, z, Colors::Red);
}