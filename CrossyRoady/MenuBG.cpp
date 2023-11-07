#include "MenuBG.h"

using namespace ConsoleGame;

bool MenuBG::IsUnmounted() { return Unmounted; }

void MenuBG::Init() {}

void MenuBG::Mount() { Unmounted = false; }

void MenuBG::Update(float deltaTime) {}

void MenuBG::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    canvas->Clear((Color)6);
}

void MenuBG::Unmount() { Unmounted = true; }
