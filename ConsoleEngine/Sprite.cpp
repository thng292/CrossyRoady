#include "Sprite.h"

using namespace ConsoleGame;

ConsoleGame::Sprite::Sprite(Vec2 dim)
    : dim(dim), data(dim.width * dim.height, Color::C_TRANSPARENT)
{
}

ConsoleGame::Sprite::Sprite(std::filesystem::path filePath) { Load(filePath); }

ConsoleGame::Sprite::Sprite(const Sprite& other)
{
    dim = other.dim;
    data = other.data;
}

ConsoleGame::Sprite::Sprite(Sprite&& other) noexcept
{
    dim = other.dim;
    data = std::move(other.data);
}

const Sprite& ConsoleGame::Sprite::operator=(const Sprite& other)
{
    dim = other.dim;
    data = other.data;
    return *this;
}

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
    int i = 0;
    if (coord.y < 0) {
        i = -coord.y;
    }
    if (i + coord.y >= _ScreenSize.height) {
        return;
    }
    for (; i < dim.height; i++) {
        int j = 0;
        if (coord.x < 0) {
            j = -coord.x;
        }
        for (; j < dim.width; j++) {
            if (j + coord.x >= _ScreenSize.width) {
                continue;
            }
            (*canvas)[i + coord.y][j + coord.x] = data[i * dim.width + j];
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
