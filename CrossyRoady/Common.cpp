#include "Common.h"

#include <format>
using namespace ConsoleGame;

std::string SecondsToHMStr(std::chrono::seconds time)
{
    auto min = time.count() / 60;
    auto hour = min / 60;
    min %= 60;
    return std::format("{}h{}", hour, min);
}

void DrawRhombus(AbstractCanvas* canvas, Vec2 center, uint8_t R, Color color)
{
    int y = center.y;
    int ny = center.y;
    for (int x = center.x - R; x < center.x; x++, y++, ny--) {
        for (int i = ny; i <= y; i++) {
            (*canvas)[i][x] = color;
        }
    }

    y = center.y - R;
    ny = center.y + R;
    for (int x = center.x; x <= center.x + R; x++, y++, ny--) {
        for (int i = y; i <= ny; i++) {
            (*canvas)[i][x] = color;
        }
    }
}
