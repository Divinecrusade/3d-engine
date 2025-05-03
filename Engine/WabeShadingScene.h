#pragma once

#include "IScene.h"
#include "Plain.h"
#include "Keyboard.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "TextureWaveShadingEffect.h"


class WabeShadingScene : public IScene
{
public:

	WabeShadingScene(Graphics& gfx)
		:
		pip{ gfx, TextureWaveShadingEffect{ Surface::FromFile(L"Images\\sauron-bhole-100x100.png") } },
		plain{ Plain::GetSkinned<ClampEffect::Vertex>(10.f) }
	{
		pip.effect.vs.SaveTranslation(Vec3{ 0.0f, 0.0f, 1.f });
		pip.effect.vs.SaveRotation(Mat3::RotationY(PI / 4.f) * Mat3::RotationZ(PI / 4.f) * Mat3::RotationX(PI / 2.f));
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

		if (kbd.KeyIsPressed('Z'))
		{
			pip.effect.gs.RotateLight(Mat3::RotationY(-dTheta * dt));
		}
		if (kbd.KeyIsPressed('X'))
		{
			pip.effect.gs.RotateLight(Mat3::RotationY(dTheta * dt));
		}

		pip.effect.vs.UpdateTime(dt);
	}
	void Draw()
	{
		pip.BeginFrame();
		pip.Draw(plain);
	}

private:

	Pipeline<TextureWaveShadingEffect> pip;
	IndexedTriangleList<TextureWaveShadingEffect::Vertex> plain;
	static constexpr float dTheta = PI;
};