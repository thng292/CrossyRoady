#pragma once
#include "ConsoleGame.h"

class Charac {
    ConsoleGame::Vec2 coord,size;

   public:
       ConsoleGame::Vec2 getCoord() { return coord;
    }

    ConsoleGame::Vec2 getSize() { return size; }
};
