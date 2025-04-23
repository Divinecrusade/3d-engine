#pragma once

#include "ClampEffect.h"
#include <cmath>

class WaveEffect
{
public:

    using Vertex = ClampEffect::Vertex;

    using PixelShader = ClampEffect::PixelShader;
    class VertexShader : public DefaultVertexShader<Vertex>
    {
    public:

        OutVertex operator()(InVertex const& v) const override
        {
            auto vert{ DefaultVertexShader<Vertex>::operator()(v) };
            vert.model_pos.y = wave_length_factor * std::sinf(time * wave_shift_factor + vert.model_pos.x * wave_amplitude_factor);
            return vert;        
        }

        void UpdateTime(float delta_time)
        {
            time = std::fmodf(time + delta_time, std::numeric_limits<float>::max());
        }

    private:
        
        float time{ 0.f };
        static constexpr float wave_length_factor{ 0.2f };
        static constexpr float wave_shift_factor{ 4.f };
        static constexpr float wave_amplitude_factor{ 5.f };
    };

public:

    WaveEffect(Surface texture)
        :
        ps{ std::move(texture) }
    {
    }

public:

    PixelShader ps;
    VertexShader vs{ };
};