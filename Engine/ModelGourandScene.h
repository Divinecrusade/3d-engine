#pragma once

#include "IScene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "GouraudShadingEffect.h"


class ModelGourandScene : public IScene
{
public:

	ModelGourandScene(Graphics& gfx, std::string const& filename)
		:
		ModelGourandScene{ gfx, IndexedTriangleList<GourandShadingEffect::Vertex>::LoadWithNormals(filename) }
	{
	}

	ModelGourandScene(Graphics& gfx, IndexedTriangleList<GourandShadingEffect::Vertex> model)
		:
		pip{ gfx, GourandShadingEffect{ } },
		model{ std::move(model) }
	{
		pip.effect.vs.SaveTranslation(Vec3{ 0.0f, 0.0f, 3.f });
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
		pip.Draw(model);
	}

private:

	Pipeline<GourandShadingEffect> pip;
	IndexedTriangleList<GourandShadingEffect::Vertex> model;
	static constexpr char const* MODELS_DIR = "./";
	static constexpr float dTheta = PI;
};