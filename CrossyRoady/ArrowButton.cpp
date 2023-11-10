#include "ArrowButton.h"

ArrowButton::ArrowButton(
    SurfaceArgs surfaceArgs, bool direction
)
    : Surface(surfaceArgs),  // no size, only conrnerSize
      direction(direction)   //"left" or "right"
{
}

double crossProduct(
    ConsoleGame::Vec2 A, ConsoleGame::Vec2 B, ConsoleGame::Vec2 C
)
{
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

bool ArrowButton::IsHover(ConsoleGame::Vec2 mousePos) const
{
    ConsoleGame::Vec2 A, B, C, D;
    if (direction == 1) {
        A = {props.pos.x, props.pos.y};
        B = {props.pos.x, props.pos.y + props.cornerSize * 2 - 1};
        C = {
            props.pos.x + props.cornerSize, props.pos.y + props.cornerSize - 1};
        D = {props.pos.x + props.cornerSize, props.pos.y + props.cornerSize};
    } else if (direction == 0) {
        A = {props.pos.x + props.cornerSize, props.pos.y};
        B = {
            props.pos.x + props.cornerSize,
            props.pos.y + props.cornerSize * 2 - 1};
        C = {props.pos.x, props.pos.y + props.cornerSize - 1};
        D = {props.pos.x, props.pos.y + props.cornerSize};
    }
    double ABC = abs(crossProduct(A, B, C));
    double PAB = abs(crossProduct(mousePos, A, B));
    double PAC = abs(crossProduct(mousePos, A, C));
    double PBC = abs(crossProduct(mousePos, B, C));

    double ACD = abs(crossProduct(A, C, D));
    double PAD = abs(crossProduct(mousePos, A, D));
    double PCD = abs(crossProduct(mousePos, C, D));
    double PDA = abs(crossProduct(mousePos, D, A));
    double totalArea = ABC + ACD;

    if (PAB + PAC + PBC == ABC && PAD + PCD + PDA == ACD &&
        PAB + PAC + PBC + PAD + PCD + PDA == totalArea) {
        return true;
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
