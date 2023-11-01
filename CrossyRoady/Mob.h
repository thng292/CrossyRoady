#pragma once
#include "ConsoleGame.h"

class Mob  //: public EntityAni
{
    ConsoleGame::Vec2 size{.width = 10, .height = 10};

   public:
    Mob() = default;

    // void setSize(ConsoleGame::Vec2 sizi) const { size = sizi; }

    ConsoleGame::Vec2 getSize() { return size; }
};
