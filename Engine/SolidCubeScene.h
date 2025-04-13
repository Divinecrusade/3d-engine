#pragma once

#include "IScene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "SolidColorEffect.h"


class SolidCubeScene : public IScene
{
public:

	SolidCubeScene(Graphics& gfx)
		:
		pip{ gfx, SolidColorEffect{ } }
	{
		pip.SaveTranslation(Vec3{ 0.0f, 0.0f, 2.f });
	}

	void Update(Keyboard& kbd, float dt)
	{
		if (kbd.KeyIsPressed('Q'))
		{
			pip.SaveRotation(Mat3::RotationX(dTheta * dt));
		}
		if (kbd.KeyIsPressed('W'))
		{
			pip.SaveRotation(Mat3::RotationY(dTheta * dt));
		}
		if (kbd.KeyIsPressed('E'))
		{
			pip.SaveRotation(Mat3::RotationZ(dTheta * dt));
		}
		if (kbd.KeyIsPressed('A'))
		{
			pip.SaveRotation(Mat3::RotationX(dTheta * dt));
		}
		if (kbd.KeyIsPressed('S'))
		{
			pip.SaveRotation(Mat3::RotationY(dTheta * dt));

		}
		if (kbd.KeyIsPressed('D'))
		{
			pip.SaveRotation(Mat3::RotationZ(dTheta * dt));
		}

		if (kbd.KeyIsPressed('R'))
		{
			pip.SaveTranslation(Vec3{ 0.0f, 0.0f, -2.0f * dt });
		}
		if (kbd.KeyIsPressed('F'))
		{
			pip.SaveTranslation(Vec3{ 0.0f, 0.0f, 2.0f * dt });
		}
	}
	void Draw()
	{
		auto model{ Cube::GetTriangles<SolidColorEffect::ColorBindedVertex>(1.f) };
		constexpr Color c[]
		{
			Colors::Red, Colors::Green, Colors::Blue, Colors::White, Colors::Yellow, Colors::Magenta
		};
		for (int i{ 0 }; i != model.vertices.size(); ++i)
		{
			model.vertices[i].color = c[i / 4];
		}

		pip.Draw(model);
	}

private:

	Pipeline<SolidColorEffect> pip;
	static constexpr float dTheta = PI;
};