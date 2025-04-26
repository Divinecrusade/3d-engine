#pragma once

#include "IScene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "SolidShadingEffect.h"


class SolidShadingSceneV : public IScene
{
public:

	SolidShadingSceneV(Graphics& gfx)
		:
		pip{ gfx, SolidShadingEffectV{ } }
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

		if (kbd.KeyIsPressed('Z'))
		{
			pip.effect.vs.RotateLight(Mat3::RotationY(-dTheta * dt));
		}
		if (kbd.KeyIsPressed('X'))
		{
			pip.effect.vs.RotateLight(Mat3::RotationY(dTheta * dt));
		}
	}
	void Draw()
	{
		pip.BeginFrame();
		auto model{ Cube::GetTrianglesWithNormals<SolidShadingEffectV::VertexWithNormale>(1.f) };
		pip.Draw(model);
	}

private:

	Pipeline<SolidShadingEffectV> pip;
	static constexpr float dTheta = PI;
};


class SolidShadingSceneG : public IScene
{
public:

	SolidShadingSceneG(Graphics& gfx)
		:
		pip{ gfx, SolidShadingEffectG{ } }
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
		auto model{ Cube::GetTriangles<SolidShadingEffectG::Vertex>(1.f) };
		pip.Draw(model);
	}

private:

	Pipeline<SolidShadingEffectG> pip;
	static constexpr float dTheta = PI;
};