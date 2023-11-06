#include "Sprite.h"

using namespace ConsoleGame;

ConsoleGame::Sprite::Sprite(Vec2 dim)
    : dim(dim), data(dim.width * dim.height, Color::C_TRANSPARENT)
{
}

ConsoleGame::Sprite::Sprite(std::filesystem::path filePath) { Load(filePath); }

void ConsoleGame::Sprite::Load(std::filesystem::path filePath)
{
    openFile = filePath;
    std::ifstream inFile(filePath, std::ios::in | std::ios::binary);
    uint16_t buff = 0;
    inFile.read((char*)&buff, 2);
    dim.width = BigEndianToHost(buff);
    inFile.read((char*)&buff, 2);
    dim.height = BigEndianToHost(buff);
    data.resize((size_t)dim.width * dim.height);
    inFile.read((char*)data.data(), sizeof(Color) * dim.width * dim.height);
}

void ConsoleGame::Sprite::Clear()
{
    std::fill(data.begin(), data.end(), Color::BRIGHT_WHITE);
}

void ConsoleGame::Sprite::Paint(AbstractCanvas* canvas, Vec2 coord) const
{
    if (coord.x >= _CanvasSize.width || coord.y >= _CanvasSize.height) {
        return;
    }
    if (coord.x + dim.width < 0 || coord.y + dim.height < 0) {
        return;
    }

    int pX = coord.x;
    int pY = coord.y;
    int left = 0;
    if (coord.x < 0) {
        left = -coord.x;
        pX = 0;
    }
    int top = 0;
    if (coord.y < 0) {
        top = -coord.y;
        pY = 0;
    }
    int right = std::min(_CanvasSize.width, coord.x + dim.width);
    int bottom = std::min(_CanvasSize.height, coord.y + dim.height);

    for (int i = top, tmpY = pY; i < bottom; i++, tmpY++) {
        for (int j = left, tmpX = pX; j < right; j++, tmpX++) {
            (*canvas)[tmpY][tmpX] = data[i * dim.width + j];
        }
    }
}

Vec2 ConsoleGame::Sprite::GetDim() const { return dim; }

const std::vector<Color>& ConsoleGame::Sprite::GetData() const { return data; }

Color* ConsoleGame::Sprite::operator[](size_t index)
{
    return data.data() + index * dim.x;
}

void ConsoleGame::Sprite::Save(std::filesystem::path filePath)
{
    std::ofstream outFile(filePath, std::ios::out | std::ios::binary);
    unsigned short buff = dim.width;
    buff = HostToBigEndian(buff);
    outFile.write((char*)&buff, 2);
    buff = dim.height;
    buff = HostToBigEndian(buff);
    outFile.write((char*)&buff, 2);
    outFile.write((char*)data.data(), data.size());
}
