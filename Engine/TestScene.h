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
      : floor_texture{Surface::FromFile(std::wstring{FLOOR_TEXTURE_URL})},
        zbuffer{std::make_shared<std::unique_ptr<float[]>>(std::make_unique<float[]>(gfx.ScreenWidth * gfx.ScreenHeight))},
        pipe_static_planes{gfx, TextureLightEffect{}, zbuffer},
        floor_object{Plain::GetSkinnedWithNormals<TextureLightEffect::TextureBindedVertex>(FLOOR_TESSALATION, FLOOR_TESSALATION, FLOOR_WIDTH, FLOOR_HEIGHT)} { 
    pipe_static_planes.effect.vs.BindProjection(PROJECTION);
    cur_view = DEFAULT_VIEW;
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
    pipe_static_planes.effect.vs.SetLightPosition(light_pos);
  }

  void Draw() {
    pipe_static_planes.BeginFrame();

    pipe_static_planes.effect.ps.BindTexture(floor_texture);
    pipe_static_planes.effect.vs.BindWorldView(FLOOR_POS * cur_view);
    pipe_static_planes.Draw(floor_object);
  }

 private:
  static constexpr wchar_t const* FLOOR_TEXTURE_URL{L"Images\\floor.png"};

  Surface const floor_texture;

  std::shared_ptr<std::unique_ptr<float[]>> zbuffer;
  Pipeline<TextureLightEffect> pipe_static_planes;
  IndexedTriangleList<TextureLightEffect::TextureBindedVertex> floor_object;
  Vec3 light_pos{0.f, 0.f, 3.5f};

  static constexpr float dTheta = PI;
  static constexpr float wFOV = 90.f;
  static constexpr float screen_ratio = 4.f / 3.f;
  static constexpr float hFOV = wFOV / screen_ratio;
  static constexpr float _near = 1.f;
  static constexpr float _far = 8.f;
  Mat4 const PROJECTION{Mat4::PerspectiveProjectionFromFOV(wFOV, screen_ratio, _near, _far)};

  Mat4 const DEFAULT_VIEW{Mat4::Identity() * Mat4::Translation({0.f, 2.f, 0.f}) * Mat4::RotationX(PI / 3)};
  Mat4 cur_view{Mat4::Identity()};

  static constexpr float FLOOR_WIDTH = 8.f;
  static constexpr float FLOOR_HEIGHT = FLOOR_WIDTH * 4.f / 3.f;
  static constexpr std::size_t FLOOR_TESSALATION = 10ull;
  Mat4 const FLOOR_POS{Mat4::Identity() * Mat4::Translation({0.f, 0.f, 4.f})};

  float dtheta = PI / 4.f;
  float thetaX = 0.f;
};