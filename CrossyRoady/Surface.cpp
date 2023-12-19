#include "Surface.h"

using namespace ConsoleGame;

Surface::Surface(SurfaceArgs args) : props(args) {}

bool Surface::IsHover(ConsoleGame::Vec2 mousePos) const
{
    int x = mousePos.x - props.pos.x;
    int y = mousePos.y - props.pos.y;

    bool tmp1 = y >= props.size.height - props.cornerSize &&
                x >= props.size.width - props.cornerSize + y;
    bool tmp2 = y <= props.cornerSize && x <= props.cornerSize - y;

    return x >= 0 && y >= 0 && x <= props.size.width &&
           y <= props.size.height && !tmp1 && !tmp2;
}

void Surface::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    int offStart = props.cornerSize;
    int offEnd = 0;
    if (props.background != C_TRANSPARENT) {
        for (int i = props.pos.y; i < props.pos.y + props.size.height; i++) {
            int startX = props.pos.x + offStart;
            if (offStart > 0 && i < props.pos.y + props.cornerSize) {
                offStart--;
            }
            int endX = props.pos.x + props.size.width - offEnd;
            if (i > props.pos.y + props.size.height - props.cornerSize) {
                offEnd++;
            }
            for (int j = startX; j < endX; j++) {
                (*canvas)[i][j] = props.background;
            }
        }
    }
    // Draw border
    if (props.hasBorder && props.border != C_TRANSPARENT) {
        // Top
        for (int i = props.pos.x + props.cornerSize;
             i < props.pos.x + props.size.width;
             i++) {
            (*canvas)[props.pos.y][i] = props.border;
        }
        // Bottom
        for (int i = props.pos.x;
             i <= props.pos.x + props.size.width - props.cornerSize;
             i++) {
            (*canvas)[props.pos.y + props.size.height][i] = props.border;
            (*canvas)[props.pos.y + props.size.height - 1][i] = props.border;
        }
        // Left
        for (int i = props.pos.y + props.cornerSize;
             i < props.pos.y + props.size.height;
             i++) {
            (*canvas)[i][props.pos.x] = props.border;
        }

        // Right
        for (int i = props.pos.y;
             i <= props.pos.y + props.size.height - props.cornerSize;
             i++) {
            (*canvas)[i][props.pos.x + props.size.width] = props.border;
        }

        // I hate handling corners
        int oriX = props.pos.x;
        int oriY = props.pos.y + props.cornerSize;
        for (int i = 0; i < props.cornerSize; i++) {
            (*canvas)[oriY][oriX] = props.border;
            oriX++;
            oriY--;
        }

        oriX = props.pos.x + props.size.width - props.cornerSize;
        oriY = props.pos.y + props.size.height;
        for (int i = 0; i < props.cornerSize; i++) {
            (*canvas)[oriY][oriX] = props.border;
            (*canvas)[oriY - 1][oriX] = props.border;
            (*canvas)[oriY - 2][oriX] = props.border;
            oriX++;
            oriY--;
        }
    }
}
