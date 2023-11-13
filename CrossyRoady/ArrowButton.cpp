#include "ArrowButton.h"
using namespace ConsoleGame;

ArrowButton::ArrowButton(SurfaceArgs surfaceArgs, bool isRight)
    : Surface(surfaceArgs),  // no size, only conrnerSize
      direction(isRight)     //"left" or "right"
{
}

bool ArrowButton::IsHover(ConsoleGame::Vec2 mousePos) const
{
    int x = mousePos.x - props.pos.x;
    int y = mousePos.y - props.pos.y;
    if (x < 0 or y < 0) {
        return false;
    }
    if (x > 2 * props.cornerSize or y > 2 * props.cornerSize) {
        return false;
    }
    if (direction == 1) {
        if (y <= props.cornerSize) {
            return x <= y;
        }
        return x <= props.cornerSize * 2 - y;
    } else {
        if (y <= props.cornerSize) {
            return x >= props.cornerSize - y;
        }
        return x >= y - props.cornerSize;
    }
    return false;
}

void ArrowButton::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    if (direction == 0) {
        int offStart = props.cornerSize;
        int offStart2 = props.cornerSize;
        int offEnd = 0;
        {
            for (int i = props.pos.y; i < props.pos.y + props.cornerSize; i++) {
                int startX = props.pos.x + offStart;
                if (offStart > 0 && i < props.pos.y + props.cornerSize) {
                    offStart--;
                }
                int startX2 = props.pos.x - offStart2 + props.cornerSize;
                if (offStart2 > 0 && i >= props.pos.y) {
                    offStart2--;
                }
                int endX = props.pos.x + props.cornerSize;
                for (int j = startX; j <= endX; j++) {
                    (*canvas)[i][j - 1] = props.background;
                }
                for (int j = startX2; j < endX; j++) {
                    (*canvas)[i + props.cornerSize][j] = props.background;
                }
            }
        }
        int oriX = props.pos.x;
        int oriY = props.pos.y + props.cornerSize - 1;
        for (int i = 0; i < props.cornerSize; i++) {
            (*canvas)[oriY][oriX] = props.border;
            oriX++;
            oriY--;
        }
        oriX = props.pos.x;
        oriY = props.pos.y + props.cornerSize;
        for (int i = 0; i < props.cornerSize; i++) {
            (*canvas)[oriY][oriX] = props.border;
            oriX++;
            oriY++;
        }
        for (int i = props.pos.y; i < props.pos.y + props.cornerSize * 2; i++) {
            (*canvas)[i][props.pos.x + props.cornerSize] = props.border;
        }
    } else if (direction == 1) {
        int offStart = 0;
        int offStart2 = props.cornerSize;
        for (int i = props.pos.y; i < props.pos.y + props.cornerSize; i++) {
            int startX = props.pos.x + offStart;
            if (offStart >= 0 && i < props.pos.y + props.cornerSize) {
                offStart++;
            }
            for (int j = props.pos.x; j <= startX; j++) {
                (*canvas)[i][j + 1] = props.background;
            }
            int startX2 = props.pos.x + offStart2;
            if (offStart2 >= 0 && i < props.pos.y + props.cornerSize) {
                offStart2--;
            }
            for (int j = props.pos.x; j < startX2; j++) {
                (*canvas)[i + props.cornerSize][j + 1] = props.background;
            }
        }
        int oriX = props.pos.x + 1;
        int oriY = props.pos.y;
        for (int i = 0; i < props.cornerSize; i++) {
            (*canvas)[oriY][oriX] = props.border;
            oriX++;
            oriY++;
        }
        oriX = props.pos.x + props.cornerSize;
        oriY = props.pos.y + props.cornerSize;
        for (int i = 0; i < props.cornerSize; i++) {
            (*canvas)[oriY][oriX] = props.border;
            oriX--;
            oriY++;
        }
        for (int i = props.pos.y; i < props.pos.y + props.cornerSize * 2; i++) {
            (*canvas)[i][props.pos.x] = props.border;
        }
    }
}

void ArrowButton::ChangeColor(
    ConsoleGame::Color backgroundColor, ConsoleGame::Color borderColor
)
{
    this->props.background = backgroundColor;
    this->props.border = borderColor;
}
