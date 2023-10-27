#include "Surface.h"

using namespace ConsoleGame;

Surface::Surface(SurfaceArgs args) : props(args) {}

float sign(Vec2 p1, Vec2 p2, Vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointNotInTriangle(Vec2 pt, Vec2 v1, Vec2 v2, Vec2 v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

bool Surface::IsHover(ConsoleGame::Vec2 mousePos)
{
    int x = mousePos.x - props.pos.x;
    int y = mousePos.y - props.pos.y;

    return x >= 0 && y >= 0 && x <= props.size.width &&
           y <= props.size.height &&
           PointNotInTriangle(
               {x, y}, {0, 0}, {0, props.cornerSize}, {props.cornerSize, 0}
           ) &&
           PointNotInTriangle(
               {x, y},
               {props.size.width - props.cornerSize, 0},
               {0, props.size.height - props.cornerSize},
               {props.cornerSize, 0}
           );
}

void Surface::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    int offStart = props.cornerSize;
    int offEnd = 0;
    if (props.background != Color::C_TRANSPARENT) {
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
    if (props.hasBorder && props.border != Color::C_TRANSPARENT) {
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
