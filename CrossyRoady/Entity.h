#pragma once
#include "ConsoleGame.h"

class Entity {
   protected:
    ConsoleGame::Vec2 coord;
    ConsoleGame::Vec2 size;

   public:
    Entity(ConsoleGame::Vec2 Coord, ConsoleGame::Vec2 Size)
        : coord(Coord), size(Size){};

    Entity(Entity& mo)
    {
        coord = mo.coord;
        size = mo.size;
    }

    ConsoleGame::Vec2 getSize();
    ConsoleGame::Vec2 getCoord();
    void setSize(ConsoleGame::Vec2 Size);
    void setCoord(ConsoleGame::Vec2 Coord);
};
