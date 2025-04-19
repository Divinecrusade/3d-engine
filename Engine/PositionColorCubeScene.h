#pragma once

#include "IScene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "PositionColorEffect.h"


class PositionColorCubeScene : public IScene
{
public:

	PositionColorCubeScene(Graphics& gfx)
		:
		pip{ gfx, PositionColorEffect{ } }
	{
		pip.effect.vs.SaveTranslation(Vec3{ 0.0f, 0.0f, 2.f });
	}

	void Update(Keyboard& kbd, float dt)
	{
		if (kbd.KeyIsPressed('Q'))
		{
			pip.effect.vs.SaveRotation(Mat3::RotationX(dTheta * dt));
		}
		if (kbd.KeyIsPressed('W'))
		{
			pip.effect.vs.SaveRotation(Mat3::RotationY(dTheta * dt));
		}
		if (kbd.KeyIsPressed('E'))
		{
			pip.effect.vs.SaveRotation(Mat3::RotationZ(dTheta * dt));
		}
		if (kbd.KeyIsPressed('A'))
		{
			pip.effect.vs.SaveRotation(Mat3::RotationX(-dTheta * dt));
		}
		if (kbd.KeyIsPressed('S'))
		{
			pip.effect.vs.SaveRotation(Mat3::RotationY(-dTheta * dt));

		}
		if (kbd.KeyIsPressed('D'))
		{
			pip.effect.vs.SaveRotation(Mat3::RotationZ(-dTheta * dt));
		}

		if (kbd.KeyIsPressed('R'))
		{
			pip.effect.vs.SaveTranslation(Vec3{ 0.0f, 0.0f, -2.0f * dt });
		}
		if (kbd.KeyIsPressed('F'))
		{
			pip.effect.vs.SaveTranslation(Vec3{ 0.0f, 0.0f, 2.0f * dt });
		}
	}
	void Draw()
	{
		auto model{ Cube::GetTriangles<Vec3>(1.f) };
		pip.BeginFrame();

		pip.Draw(model);
	}

private:

	Pipeline<PositionColorEffect> pip;
	static constexpr float dTheta = PI;
};