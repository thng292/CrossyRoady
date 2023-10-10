#include "Common.h"

bool ConsoleGame::IsKeyDown(int key)
{
    return (GetAsyncKeyState(key) & (1 << 16));
}
