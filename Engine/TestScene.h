#pragma once

#include "ChiliMath.h"
#include "IScene.h"
#include "Pipeline.h"
#include "TextureLightEffect.h"
#include "Plain.h"
#include "Mouse.h"

class TestScene : public IScene {
 public:
  TestScene(Graphics& gfx)
      : zbuffer{std::make_shared<std::unique_ptr<float[]>>(std::make_unique<float[]>(gfx.ScreenWidth * gfx.ScreenHeight))},
        pipe_floor{gfx, TextureLightEffect{L"Images/floor.png"}, zbuffer},
        floor_object{Plain::GetSkinnedWithNormals<TextureLightEffect::TextureBindedVertex>(
            4, 4.f, 5.f, 5.f, 2.f)} { 
    pipe_floor.effect.vs.BindProjection(projection);
    //pipe_floor.effect.ps.SetLightPosition(Vec3{0.f, 0.f, 3.5f});
  }

  void Update(Keyboard& kbd, Mouse& mouse, float dt) {
    if (kbd.KeyIsPressed('W')) {
      thetaX += dt * dtheta;
      OutputDebugString(std::to_wstring(thetaX * 180 / PI).data());
      OutputDebugString(L"\n");
    }
    if (kbd.KeyIsPressed('S')) {
      thetaX -= dt * dtheta;
      OutputDebugString(std::to_wstring(thetaX * 180 / PI).data());
      OutputDebugString(L"\n");
    }
    pipe_floor.effect.vs.BindWorldView(Mat4::RotationX(thetaX) * Mat4::Translation({0.f, -1.f, 4.f}));
  }

  void Draw() {
    pipe_floor.BeginFrame();
    pipe_floor.Draw(floor_object);
  }

 private:
  std::shared_ptr<std::unique_ptr<float[]>> zbuffer;
  Pipeline<TextureLightEffect> pipe_floor;
  IndexedTriangleList<TextureLightEffect::TextureBindedVertex> floor_object;
  Vec3 light_pos{0.f, 0.f, 0.f};

  static constexpr float dTheta = PI;
  static constexpr float wFOV = 90.f;
  static constexpr float screen_ratio = 4.f / 3.f;
  static constexpr float hFOV = wFOV / screen_ratio;
  static constexpr float _near = 1.f;
  static constexpr float _far = 6.f;
  Mat4 projection{Mat4::PerspectiveProjectionFromFOV(wFOV, screen_ratio, _near, _far)};

  float dtheta = PI / 4.f;
  float thetaX = 0.f;
};