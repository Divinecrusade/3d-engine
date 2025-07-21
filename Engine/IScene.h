#pragma once

#include "Keyboard.h"

__interface IScene
{
public:

    virtual void Update(Keyboard& kbd, float dt, Mouse& mouse) = 0;
    virtual void Draw() = 0;
};