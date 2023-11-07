#pragma once
#include "ConsoleGame.h"

class MenuBG {
   public:
    void Init();
    void Mount();
    void Update(float deltaTime);
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
    void Unmount();
};
