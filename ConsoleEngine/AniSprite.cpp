#include "AniSprite.h"

#include <algorithm>
#include <fstream>

namespace ConsoleGame {
    void AniSprite::findHitBox()
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

    AniSprite::AniSprite(std::filesystem::path path) { Load(path); }

    const Box& AniSprite::GetHitBox() const { return hitBox; }

    void AniSprite::EditHitBox(Vec2 coordOffset, Vec2 dimOffset)
    {
        hitBox.coord.x += coordOffset.x;
        hitBox.coord.y += coordOffset.y;

        hitBox.dim.width += dimOffset.width;
        hitBox.dim.height += dimOffset.height;
    }

    Vec2 AniSprite::GetDim() const { return dim; }

    const std::vector<Color>& AniSprite::GetData() const { return data; }

    float AniSprite::GetFrameDuration() { return frameDuration; }

    void AniSprite::SetFrameDuration(float dur) { frameDuration = dur; }

    void AniSprite::Play(bool repeat)
    {
        playing      = true;
        this->repeat = repeat;
    }

    bool AniSprite::IsPlaying() const { return playing; }

    void AniSprite::Stop() { playing = false; }

    void AniSprite::Load(std::filesystem::path path)
    {
        std::ifstream file(path, std::ios::in | std::ios::binary);
        uint16_t buff = 0;
        file.read((char*)&buff, sizeof(buff));
        dim.width = BigEndianToHost(buff);
        file.read((char*)&buff, sizeof(buff));
        dim.height = BigEndianToHost(buff);
        file.read((char*)&totalFrame, sizeof(totalFrame));
        totalFrame = BigEndianToHost(totalFrame);
        uint8_t fps;
        file.read((char*)&fps, sizeof(fps));
        frameDuration = 1.0f / fps;
        data.resize(dim.width * dim.height * totalFrame);
        file.read((char*)data.data(), data.size());
        findHitBox();
    }

    void AniSprite::Draw(AbstractCanvas* canvas, Vec2 coord) const
    {
        if (coord.x >= _CanvasSize.width || coord.y >= _CanvasSize.height) {
            return;
        }
        if (coord.x + dim.width < 0 || coord.y + dim.height < 0) {
            return;
        }

        const size_t frameOffset =
            size_t(dim.width) * dim.height * playingFrame;

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
                if (data[frameOffset + i * dim.width + j] !=
                    Color::C_TRANSPARENT) {
                    (*canvas)[tmpY][tmpX] =
                        data[frameOffset + i * dim.width + j];
                }
            }
        }
    }

    void AniSprite::ResetFrame()
    {
        playingFrame = 0;
        timePassed   = 0;
    }

    void AniSprite::AdvanceFrame()
    {
        playingFrame = (playingFrame + 1) % totalFrame;
    }

    void AniSprite::AutoUpdateFrame(float deltaTime)
    {
        if (!playing) {
            return;
        }
        timePassed += deltaTime;
        playingFrame = timePassed / frameDuration;
        if (playingFrame >= totalFrame) {
            timePassed -= frameDuration * totalFrame;
            if (!repeat) {
                playingFrame = totalFrame - 1;
                Stop();
            } else {
                playingFrame = 0;
            }
        }
    }

    void AniSprite::Unload() { data.clear(); }
}  // namespace ConsoleGame
