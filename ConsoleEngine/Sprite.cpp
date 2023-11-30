#include "Sprite.h"

namespace ConsoleGame {
    void Sprite::findHitBox()
    {
        hitBox.dim   = dim;
        hitBox.coord = {.x = 0, .y = 0};
        bool tmp     = true;
        for (int top = 0; top < dim.height and tmp; top++) {
            for (int i = 0; i < dim.width; i++) {
                if (data[top * dim.width + i] != Color::C_TRANSPARENT) {
                    tmp = false;
                    break;
                }
            }
            hitBox.coord.y++;
            hitBox.dim.height--;
        }
        tmp = true;
        for (int bottom = dim.height - 1; bottom >= 0 and tmp; bottom--) {
            for (int i = 0; i < dim.width; i++) {
                if (data[bottom * dim.width + i] != Color::C_TRANSPARENT) {
                    tmp = false;
                    break;
                }
            }
            hitBox.dim.height--;
        }

        tmp = true;
        for (int left = 0; left < dim.width and tmp; left++) {
            for (int i = 0; i < dim.height; i++) {
                if (data[i * dim.width + left] != Color::C_TRANSPARENT) {
                    tmp = false;
                    break;
                }
            }
            hitBox.dim.width--;
            hitBox.coord.x++;
        }

        tmp = true;
        for (int right = dim.width - 1; right >= 0 and tmp; right--) {
            for (int i = 0; i < dim.height; i++) {
                if (data[i * dim.width + right] != Color::C_TRANSPARENT) {
                    tmp = false;
                    break;
                }
            }
            hitBox.dim.width--;
        }
    }

    Sprite::Sprite(Vec2 dim)
        : dim(dim), data(dim.width * dim.height, Color::C_TRANSPARENT)
    {
    }

    Sprite::Sprite(std::filesystem::path filePath) { Load(filePath); }

    void Sprite::Unload() { data.clear(); }

    void Sprite::Load(std::filesystem::path filePath)
    {
        std::ifstream inFile(filePath, std::ios::in | std::ios::binary);
        uint16_t buff = 0;
        inFile.read((char*)&buff, 2);
        dim.width = BigEndianToHost(buff);
        inFile.read((char*)&buff, 2);
        dim.height = BigEndianToHost(buff);
        data.resize((size_t)dim.width * dim.height);
        inFile.read((char*)data.data(), sizeof(Color) * dim.width * dim.height);
        findHitBox();
    }

    void Sprite::Clear()
    {
        std::fill(data.begin(), data.end(), Color::BRIGHT_WHITE);
    }

    void Sprite::Draw(AbstractCanvas* canvas, Vec2 coord) const
    {
        if (coord.x >= _CanvasSize.width || coord.y >= _CanvasSize.height) {
            return;
        }
        if (coord.x + dim.width < 0 || coord.y + dim.height < 0) {
            return;
        }

        int pX   = coord.x;
        int pY   = coord.y;
        int left = 0;
        if (coord.x < 0) {
            left = -coord.x;
            pX   = 0;
        }
        int top = 0;
        if (coord.y < 0) {
            top = -coord.y;
            pY  = 0;
        }
        int right  = std::min(_CanvasSize.width, coord.x + dim.width);
        int bottom = std::min(_CanvasSize.height, coord.y + dim.height);

        for (int i = top, tmpY = pY; tmpY < bottom; i++, tmpY++) {
            for (int j = left, tmpX = pX; tmpX < right; j++, tmpX++) {
                if (data[i * dim.width + j] != Color::C_TRANSPARENT) {
                    (*canvas)[tmpY][tmpX] = data[i * dim.width + j];
                }
            }
        }
    }

    Vec2 Sprite::GetDim() const { return dim; }

    const Box& Sprite::GetHitBox() const { return hitBox; }

    void Sprite::EditHitBox(Vec2 coordOffset, Vec2 dimOffset)
    {
        hitBox.coord.x += coordOffset.x;
        hitBox.coord.y += coordOffset.y;

        hitBox.dim.width += dimOffset.width;
        hitBox.dim.height += dimOffset.height;
    }

    const std::vector<Color>& Sprite::GetData() const { return data; }

    Color* Sprite::operator[](size_t index)
    {
        return data.data() + index * dim.x;
    }

    void Sprite::Save(std::filesystem::path filePath)
    {
        std::ofstream outFile(filePath, std::ios::out | std::ios::binary);
        unsigned short buff = dim.width;
        buff                = HostToBigEndian(buff);
        outFile.write((char*)&buff, 2);
        buff = dim.height;
        buff = HostToBigEndian(buff);
        outFile.write((char*)&buff, 2);
        outFile.write((char*)data.data(), data.size());
    }
}  // namespace ConsoleGame
