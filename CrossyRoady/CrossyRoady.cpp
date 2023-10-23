#include <memory>

#include "ConsoleGame.h"
using namespace ConsoleGame;

auto main() -> int
{
    Font::Load("test.font");
    // LocalStorage::LoadFromFile();
    // defer { LocalStorage::SaveToFile(); };
    auto game = std::make_unique<Game>(L"Crossy Roady", GetDisplayRefreshRate());
    return 0;
}
