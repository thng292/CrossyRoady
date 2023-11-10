#include "ArrowButton.h"
using namespace ConsoleGame;

ArrowButton::ArrowButton(SurfaceArgs surfaceArgs, bool direction)
    : Surface(surfaceArgs),  // no size, only conrnerSize
      direction(direction)   //"left" or "right"
{
}

bool ArrowButton::IsHover(ConsoleGame::Vec2 mousePos) const
{
    int x;
    int y = mousePos.y - props.pos.y;
    bool tmp1, tmp2;
    if (direction == 1) {
        x = (mousePos.x - 25) - props.pos.x;
        tmp1 = (x <= props.cornerSize - y) && (y <= props.cornerSize);
        tmp2 = (y > props.cornerSize) && (x <= props.cornerSize * 2 - y + 2);

    } else if (direction == 0) {
        x = (mousePos.x - 10) - props.pos.x;
        tmp1 = (y <= props.cornerSize) && (x >= props.cornerSize + 1 - y);
        tmp2 = (y > props.cornerSize) && (x >= y - props.cornerSize - 1);
    }
    return (x >= 0) && (y >= 0) && (x <= props.cornerSize + 1) &&
           (y <= props.cornerSize * 2) && (tmp1 || tmp2);
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
