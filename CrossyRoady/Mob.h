#pragma once
#include "ConsoleGame.h"

class Mob  //: public EntityAni
{
    ConsoleGame::Vec2 size;

   public:
    Mob() = default;

    void setSize(ConsoleGame::Vec2 size) const { size = size; }

    ConsoleGame::Vec2 getSize() { return size; }
};
