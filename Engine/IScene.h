#pragma once

#include "Keyboard.h"

__interface IScene
{
public:

    virtual void Update(Keyboard& kbd, float dt) = 0;
    virtual void Draw() = 0;
};