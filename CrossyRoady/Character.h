#pragma once

#include "ConsoleGame.h"

class Character {
   private:
    int SQUARE = 16;
    ConsoleGame::Vec2 coord{.x = 50, .y = 50};
    ConsoleGame::Vec2 hitbox{.width = 2 * SQUARE, .height = 2 * SQUARE};
    ConsoleGame::AniSprite aniSprite;
    int maxHealth, health, speed;
   public:
    
};
