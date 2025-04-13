#pragma once

#include "Cube.h"
#include "CubeExtended.h"
#include "Dice.h"
#include "Keyboard.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "ClampEffect.h"


class TexCubeScene
{
public:

	TexCubeScene(Graphics& gfx)
	:
	pip{ gfx, ClampEffect{Surface::FromFile(L"Images\\dice.png") } }
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
		pip.Draw(Dice::GetTrianglesTex(1.f));
	}

private:

	Pipeline<ClampEffect> pip;
	static constexpr float dTheta = PI;
};