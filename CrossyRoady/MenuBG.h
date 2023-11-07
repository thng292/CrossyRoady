#pragma once
#include "ConsoleGame.h"

class MenuBG {
    bool Unmounted = false;
   public:
    bool IsUnmounted();
    void Init();
    void Mount();
    void Update(float deltaTime);
    void Draw(ConsoleGame::AbstractCanvas* canvas) const;
    void Unmount();
};
