#pragma once

#include "ChiliMath.h"
#include "IScene.h"
#include "Pipeline.h"
#include "TextureLightEffect.h"
#include "SolidColorEffectH.h"
#include "PhongSpecularEffect.h"
#include "NewWaveEffect.h"
#include "Plain.h"
#include "Sphere.h"
#include "Mouse.h"

class TestScene : public IScene {
 public:
  TestScene(Graphics& gfx)
      : FLOOR_TEXTURE{Surface::FromFile(std::wstring{FLOOR_TEXTURE_URL})},
        WALL_TEXTURE{Surface::FromFile(std::wstring{WALL_TEXTURE_URL})},
        CEILING_TEXTURE{Surface::FromFile(std::wstring{CEILING_TEXTURE_URL})},
        WAVE_TEXTURE{Surface::FromFile(std::wstring{WAVE_TEXTURE_URL})},
        zbuffer{std::make_shared<std::unique_ptr<float[]>>(std::make_unique<float[]>(gfx.ScreenWidth * gfx.ScreenHeight))},
        pipe_static_planes{gfx, TextureLightEffect{}, zbuffer},
        floor_object{Plain::GetSkinnedWithNormals<TextureLightEffect::TextureBindedVertex>(FLOOR_TESSALATION, FLOOR_TESSALATION, FLOOR_WIDTH, FLOOR_HEIGHT)},
        wall_object_long{Plain::GetSkinnedWithNormals<TextureLightEffect::TextureBindedVertex>(WALL_TESSALATION, WALL_TESSALATION, WALL_WIDTH_LONG, WALL_HEIGHT)},
        wall_object_short{Plain::GetSkinnedWithNormals<TextureLightEffect::TextureBindedVertex>(WALL_TESSALATION, WALL_TESSALATION, WALL_WIDTH_SHORT, WALL_HEIGHT)},
        pipe_bulb{gfx, SolidColorEffectH{Colors::White}, zbuffer},
        bulb_object{Sphere::GetTriangles<SolidColorEffectH::Vertex>(BULB_RADIUS)},
        pipe_suzanne(gfx, PhongSpecularEffect{}, zbuffer),
        suzanne_object{IndexedTriangleList<PhongSpecularEffect::Vertex>::LoadWithNormals(SUZANNE_MODEL_URL)},
        pipe_wave{gfx, NewWaveEffect{}, zbuffer},
        wave_object{Plain::GetSkinnedWithNormals<NewWaveEffect::Vertex>(WAVE_TESSALATION, WAVE_TESSALATION, WAVE_WIDTH, WAVE_HEIGHT)} { 
    pipe_static_planes.effect.vs.BindProjection(PROJECTION);
    pipe_bulb.effect.vs.BindProjection(PROJECTION);
    pipe_suzanne.effect.vs.BindProjection(PROJECTION);
    pipe_wave.effect.vs.BindProjection(PROJECTION);
  }

  void Update(Keyboard& kbd, Mouse& mouse, float dt) {
    while (!mouse.IsEmpty()) {
      auto const e{mouse.Read()};
      switch (e.GetType()) {
        case Mouse::Event::Type::LPress:
          mouse_engaged = true;
          mouse_pos = e.GetPos();
          break;

        case Mouse::Event::Type::LRelease:
          mouse_engaged = false;
          break;

        case Mouse::Event::Type::Move:
          if (!mouse_engaged) break;

          auto const delta_pos{e.GetPos() - mouse_pos};
          camera_rot_inv = camera_rot_inv *
                           Mat4::RotationX(w_angle_delta * float(delta_pos.y)) *
                           Mat4::RotationY(h_angle_delta * float(delta_pos.x));
          mouse_pos = e.GetPos();
          break;
      }
    }

    if (kbd.KeyIsPressed('W')) {
      camera_pos += Vec4{0.f, 0.f, 1.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('S')) {
      camera_pos -= Vec4{0.f, 0.f, 1.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('A')) {
      camera_pos -= Vec4{1.f, 0.f, 0.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('D')) {
      camera_pos += Vec4{1.f, 0.f, 0.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('Z')) {
      camera_pos = Vec4{0.f, 1.f, 0.f} * !camera_rot_inv * dt * camera_speed;
    }
    if (kbd.KeyIsPressed('X')) {
      camera_pos = Vec4{0.f, 1.f, 0.f} * !camera_rot_inv * dt * camera_speed;
    }
    auto const view_offset{-camera_pos};
    cur_view = Mat4::Translation(view_offset) * camera_rot_inv;

    suzanne_spin_y_theta += SUZANNE_SPIN_SPEED * dt;
    suzanne_spin_y_theta = wrap_angle(suzanne_spin_y_theta);

    light_theta_pos = wrap_angle(light_theta_pos + LIGHT_SOURCE_SPEED * dt);
    light_pos.y = MID_Y_POS_LIGHT + std::sinf(light_theta_pos) * DELTA_Y_POS_LIGHT;

    pipe_wave.effect.vs.UpdateTime(dt);
    pipe_wave.effect.ps.BindTexture(WAVE_TEXTURE);
  }

  void Draw() {
    pipe_static_planes.BeginFrame();

    pipe_static_planes.effect.vs.SetLightPosition(light_pos * cur_view);
    pipe_static_planes.effect.ps.BindTexture(FLOOR_TEXTURE);
    pipe_static_planes.effect.vs.BindWorldView(FLOOR_WORLD_POS * cur_view);
    pipe_static_planes.Draw(floor_object);
    
    pipe_static_planes.effect.ps.BindTexture(WALL_TEXTURE);
    for (auto const& wall : WALLS_WORLD_POS) {
      auto const& pos = wall.first;
      auto const& type = wall.second;
      pipe_static_planes.effect.vs.BindWorldView(pos * cur_view);
      switch (type) {
        case WALL_TYPE::LONG: pipe_static_planes.Draw(wall_object_long); break;
        case WALL_TYPE::SHORT: pipe_static_planes.Draw(wall_object_short); break;
      }
    }

    pipe_static_planes.effect.ps.BindTexture(CEILING_TEXTURE);
    pipe_static_planes.effect.vs.BindWorldView(CEILING_WORLD_POS * cur_view);
    pipe_static_planes.Draw(floor_object);

    pipe_bulb.effect.vs.BindWorldView(Mat4::Translation(light_pos) * cur_view);
    pipe_bulb.Draw(bulb_object);

    pipe_suzanne.effect.ps.SetLightPosition(light_pos * cur_view);
    pipe_suzanne.effect.vs.BindWorldView(Mat4::RotationY(suzanne_spin_y_theta) * Mat4::Translation(SUZANNE_DEFAULT_POS) * cur_view);
    pipe_suzanne.Draw(suzanne_object);

    pipe_wave.effect.vs.SetLightPosition(light_pos * cur_view);
    pipe_wave.effect.vs.BindWorldView(WAVE_WORLD_POS * cur_view);
    pipe_wave.Draw(wave_object);
  }

 private:
  static constexpr wchar_t const* FLOOR_TEXTURE_URL{L"Images\\floor.png"};
  static constexpr wchar_t const* WALL_TEXTURE_URL{L"Images\\stonewall.png"};
  static constexpr wchar_t const* CEILING_TEXTURE_URL{L"Images\\ceiling.png"};
  static constexpr char const* SUZANNE_MODEL_URL{"suzanne.obj"};
  static constexpr wchar_t const* WAVE_TEXTURE_URL{L"Images\\sauron-bhole-100x100.png"};

  Surface const FLOOR_TEXTURE;
  Surface const WALL_TEXTURE;
  Surface const CEILING_TEXTURE;
  Surface const WAVE_TEXTURE;

  static constexpr std::size_t WALLS_COUNT{4ull};

  std::shared_ptr<std::unique_ptr<float[]>> zbuffer;
  Pipeline<TextureLightEffect> pipe_static_planes;
  IndexedTriangleList<TextureLightEffect::TextureBindedVertex> floor_object;
  IndexedTriangleList<TextureLightEffect::TextureBindedVertex> wall_object_long;
  IndexedTriangleList<TextureLightEffect::TextureBindedVertex> wall_object_short;

  Pipeline<SolidColorEffectH> pipe_bulb;
  IndexedTriangleList<SolidColorEffectH::Vertex> bulb_object;

  Pipeline<PhongSpecularEffect> pipe_suzanne;
  IndexedTriangleList<PhongSpecularEffect::Vertex> suzanne_object;

  Pipeline<NewWaveEffect> pipe_wave;
  IndexedTriangleList<NewWaveEffect::Vertex> wave_object;

  Vec4 const SUZANNE_DEFAULT_POS{1.5f, 1.25f, 2.5f};
  float suzanne_spin_y_theta{0.f};
  static constexpr float SUZANNE_SPIN_SPEED{PI / 4.f};

  static constexpr float BULB_RADIUS = 0.25f;

  static constexpr float dTheta = PI;
  static constexpr float wFOV = 90.f;
  static constexpr float screen_ratio = 4.f / 3.f;
  static constexpr float hFOV = wFOV / screen_ratio;
  static constexpr float _near = 0.2f;
  static constexpr float _far = 10.f;
  Mat4 const PROJECTION{Mat4::PerspectiveProjectionFromFOV(wFOV, screen_ratio, _near, _far)};

  static constexpr float FLOOR_WIDTH = 6.f;
  static constexpr float FLOOR_HEIGHT = 8.f;
  static constexpr std::size_t FLOOR_TESSALATION = 10ull;
  Mat4 const FLOOR_WORLD_POS{Mat4::Identity() * Mat4::RotationX(PI / 2.f)};

  static constexpr float WALL_WIDTH_LONG = FLOOR_HEIGHT;
  static constexpr float WALL_WIDTH_SHORT = FLOOR_WIDTH;
  static constexpr float WALL_HEIGHT = 5.f;
  static constexpr std::size_t WALL_TESSALATION = 10ull;
  enum WALL_TYPE {
    LONG, SHORT
  };
  std::array<std::pair<Mat4, WALL_TYPE>, WALLS_COUNT> const WALLS_WORLD_POS{
    std::pair<Mat4, WALL_TYPE>{Mat4::Identity() * Mat4::RotationY(-PI / 2.f) * Mat4::Translation({-FLOOR_WIDTH / 2.f, WALL_HEIGHT / 2.f, 0.f}), LONG},
    std::pair<Mat4, WALL_TYPE>{Mat4::Identity() * Mat4::RotationY(PI / 2.f) * Mat4::Translation({FLOOR_WIDTH / 2.f, WALL_HEIGHT / 2.f, 0.f}), LONG},
    std::pair<Mat4, WALL_TYPE>{Mat4::Identity() * Mat4::Translation({0.f, WALL_HEIGHT / 2.f, FLOOR_HEIGHT / 2.f}), SHORT},
    std::pair<Mat4, WALL_TYPE>{Mat4::Identity() * Mat4::RotationY(-PI) * Mat4::Translation({0.f, WALL_HEIGHT / 2.f, -FLOOR_HEIGHT / 2.f}), SHORT}
  };

  Mat4 const CEILING_WORLD_POS{Mat4::Identity() * Mat4::RotationX(3.f * (PI / 2.f)) * Mat4::Translation({0.f, WALL_HEIGHT, 0.f})};

  static constexpr std::size_t WAVE_TESSALATION{14ull};
  static constexpr float WAVE_WIDTH{1.f};
  static constexpr float WAVE_HEIGHT{WAVE_WIDTH};

  Mat4 const WAVE_WORLD_POS{Mat4::Scaling(2.f) * Mat4::RotationX(PI / 2.f) * Mat4::Translation({-WAVE_WIDTH - 0.2f, 0.3f, -1.2f})};

  Mat4 cur_view{Mat4::Identity()};

  static constexpr float h_angle_delta{hFOV / Graphics::ScreenHeight * PI /
                                       180.f};
  static constexpr float w_angle_delta{wFOV / Graphics::ScreenWidth * PI /
                                       180.f};
  Mat4 camera_rot_inv{Mat4::Identity() * !Mat4::RotationX(PI / 4.f)};
  Vei2 mouse_pos{};
  bool mouse_engaged{false};

  static constexpr float camera_speed{0.8f};
  Vec4 camera_pos{0.f, 1.6f, -FLOOR_HEIGHT / 2.f};

  static constexpr float MIN_Y_POS_LIGHT{0.2f};
  static constexpr float MAX_Y_POS_LIGHT{4.2f};
  static constexpr float MID_Y_POS_LIGHT{(MIN_Y_POS_LIGHT + MAX_Y_POS_LIGHT) / 2.f};
  static constexpr float DELTA_Y_POS_LIGHT{(MAX_Y_POS_LIGHT - MIN_Y_POS_LIGHT) / 2.f};
  float light_theta_pos{0.f};
  static constexpr float LIGHT_SOURCE_SPEED{PI / 2.f};
  Vec4 light_pos{0.f, 0.f, 0.f};
};