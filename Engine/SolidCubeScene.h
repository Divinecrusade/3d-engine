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
		pip.effect.vs.SaveTranslation(Vec3{ 0.0f, 0.0f, 2.f });
	}

	void Update(Keyboard& kbd, float dt)
	{
		if (kbd.KeyIsPressed('Q'))
		{
			theta = theta * Mat3::RotationX(-dTheta * dt);
			pip.effect.vs.SaveRotation(Mat3::RotationX(dTheta * dt));
		}
		if (kbd.KeyIsPressed('W'))
		{
			theta = theta * Mat3::RotationY(-dTheta * dt);
			pip.effect.vs.SaveRotation(Mat3::RotationY(dTheta * dt));
		}
		if (kbd.KeyIsPressed('E'))
		{
			theta = theta * Mat3::RotationZ(-dTheta * dt);
			pip.effect.vs.SaveRotation(Mat3::RotationZ(dTheta * dt));
		}
		if (kbd.KeyIsPressed('A'))
		{
			theta = theta * Mat3::RotationX(dTheta * dt);
			pip.effect.vs.SaveRotation(Mat3::RotationX(-dTheta * dt));
		}
		if (kbd.KeyIsPressed('S'))
		{
			theta = theta * Mat3::RotationY(dTheta * dt);
			pip.effect.vs.SaveRotation(Mat3::RotationY(-dTheta * dt));

		}
		if (kbd.KeyIsPressed('D'))
		{
			theta = theta * Mat3::RotationZ(dTheta * dt);
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
		pip.BeginFrame();

		constexpr Color c[]
		{
			Colors::Red, Colors::Green, Colors::Blue, Colors::White, Colors::Yellow, Colors::Magenta
		};

		{
			auto model{ Cube::GetTriangles<SolidColorEffect::ColorBindedVertex>(0.75f) };
			for (int i{ 0 }; i != model.vertices.size(); ++i)
			{
				model.vertices[i].color = c[i / 4];
			}
			pip.Draw(model);
		}

		{
			auto model{ Cube::GetTriangles<SolidColorEffect::ColorBindedVertex>(0.75f) };
			for (int i{ 0 }; i != model.vertices.size(); ++i)
			{
				model.vertices[i].color = c[i / 4];
			}

			auto const rot{ pip.effect.vs.GetRotation() };
			auto const translation{ pip.effect.vs.GetTranslation() };

			pip.effect.vs.SetRotation(theta);
			pip.effect.vs.SetTranslation(Vec3{ 0.0f, 0.0f, 2.f });

			pip.Draw(model);

			pip.effect.vs.SetRotation(rot);
			pip.effect.vs.SetTranslation(translation);
		}
	}

private:

	Pipeline<SolidColorEffect> pip;
	static constexpr float dTheta = PI;
	Mat3 theta = Mat3::Identity();
};