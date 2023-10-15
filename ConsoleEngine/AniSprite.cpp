#include "AniSprite.h"

#include <algorithm>
#include <fstream>

namespace ConsoleGame {

    AniSprite::AniSprite(std::filesystem::path path) { Load(path); }

    AniSprite::AniSprite(const AniSprite& other) { *this = other; }

    AniSprite::AniSprite(AniSprite&& other) noexcept
    {
        data = std::move(other.data);
        dim = other.dim;
        frameDuration = other.frameDuration;
        playingFrame = other.playingFrame;
        filePath = other.filePath;
    }

    const AniSprite AniSprite::operator=(const AniSprite& other)
    {
        data = other.data;
        filePath = other.filePath;
        timePassed = other.timePassed;
        playing = other.playing;
        playingFrame = other.playingFrame;
        frameDuration = other.frameDuration;
        dim = other.dim;
        return other;
    }

    Vec2 AniSprite::GetDim() const { return dim; }

    const std::vector<Color>& AniSprite::GetData() const { return data; }

    void AniSprite::Play(bool repeat)
    {
        playing = true;
        this->repeat = repeat;
    }

    bool AniSprite::IsPlaying() const { return playing; }

    void AniSprite::Stop() { playing = false; }

    void AniSprite::Load(std::filesystem::path path)
    {
        filePath = path;
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
    }

    void AniSprite::Paint(AbstractCanvas* canvas, Vec2 coord) const
    {
        if (coord.x >= _CanvasSize.width || coord.y >= _CanvasSize.height) {
            return;
        }
        if (coord.x + dim.width < 0 || coord.y + dim.height < 0) {
            return;
        }

        const size_t frameOffset =
            size_t(dim.width) * dim.height * playingFrame;

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
                (*canvas)[tmpY][tmpX] = data[frameOffset + i * dim.width + j];
            }
        }
    }

    void AniSprite::ResetFrame()
    {
        playingFrame = 0;
        timePassed = 0;
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
        // timePassed += deltaTime * 2; somehow this work?????????????
        timePassed += deltaTime;
        playingFrame = timePassed / frameDuration;
        if (playingFrame >= totalFrame) {
            playingFrame = 0;
            timePassed -= frameDuration * totalFrame;
            if (!repeat) {
                Stop();
            }
        }
    }
}  // namespace ConsoleGame
