#pragma once

#include "IScene.h"
#include "Sphere.h"
#include "Pipeline.h"
#include "SolidShadingEffect.h"


class SphereScene : public IScene
{
public:

	SphereScene(Graphics& gfx)
		:
		pip{ gfx, SolidShadingEffectG{ } },
		model{ Sphere::GetTriangles<Vec3>(1.f) }
	{
		pip.effect.vs.SaveTranslation(Vec3{ 0.0f, 0.0f, 1.f });
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

		if (kbd.KeyIsPressed('Z'))
		{
			pip.effect.gs.RotateLight(Mat3::RotationY(-dTheta * dt));
		}
		if (kbd.KeyIsPressed('X'))
		{
			pip.effect.gs.RotateLight(Mat3::RotationY(dTheta * dt));
		}
	}
	void Draw()
	{
		pip.BeginFrame();
		pip.Draw(model);
	}

private:

	Pipeline<SolidShadingEffectG> pip;
	IndexedTriangleList<SolidShadingEffectG::Vertex> model;
	static constexpr float dTheta = PI;
};