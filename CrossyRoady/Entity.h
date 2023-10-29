#pragma once
#include "ConsoleGame.h"

class Entity {
    ConsoleGame::Vec2 coord;
    ConsoleGame::Vec2 size;
    int velocity;

   public:
    Entity(ConsoleGame::Vec2 Coord, ConsoleGame::Vec2 Size, int V)
        : coord(Coord), size(Size), velocity(V){};

    Entity(Entity& mo) { 
        coord = mo.coord;
        size = mo.size;
        velocity = mo.velocity;
    }
    int getVelocity();
    ConsoleGame::Vec2 getSize();
    ConsoleGame::Vec2 getCoord();
    void setVelocity(int Velocity);
    void setSize(ConsoleGame::Vec2 Size);
    void setCoord(ConsoleGame::Vec2 Coord);
};
