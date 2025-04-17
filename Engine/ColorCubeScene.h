#pragma once

#include "IScene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "ColorBlendEffect.h"


class ColorCubeScene : public IScene
{
public:

	ColorCubeScene(Graphics& gfx)
		:
		pip{ gfx, ColorBlendEffect{ } }
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
			pip.effect.vs.SaveRotation(Mat3::RotationX(dTheta * dt));
		}
		if (kbd.KeyIsPressed('S'))
		{
			pip.effect.vs.SaveRotation(Mat3::RotationY(dTheta * dt));

		}
		if (kbd.KeyIsPressed('D'))
		{
			pip.effect.vs.SaveRotation(Mat3::RotationZ(dTheta * dt));
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
		auto model{ Cube::GetPlains<ColorBlendEffect::ColorBindedVertex>(1.f) };
		model.vertices[0u].color = Vec3( Colors::Black );
		model.vertices[1u].color = Vec3( Colors::Green );
		model.vertices[2u].color = Vec3( Colors::Red );
		model.vertices[3u].color = Vec3( Colors::Blue );
		model.vertices[4u].color = Vec3( Colors::Cyan );
		model.vertices[5u].color = Vec3( Colors::Magenta );
		model.vertices[6u].color = Vec3( Colors::Yellow );
		model.vertices[7u].color = Vec3( Colors::White );

		pip.BeginFrame();

		pip.Draw(model);
	}

private:

	Pipeline<ColorBlendEffect> pip;
	static constexpr float dTheta = PI;
};