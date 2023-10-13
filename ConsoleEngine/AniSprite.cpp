#include "AniSprite.h"

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

    void AniSprite::Stop() { playing = false; }

    void AniSprite::Load(std::filesystem::path path)
    {
        std::fstream file(path, std::ios::in | std::ios::binary);
        uint16_t buffs = 0;
        file.read((char*)&buffs, 2);
        dim.width = ToHost(buffs);
        file.read((char*)&buffs, 2);
        dim.height = ToHost(buffs);

        uint8_t buff = 0;
        file.read((char*)&buff, 1);
        frameDuration = std::chrono::milliseconds(buff * durationMultiplier);

        file.read((char*)&buff, 1);
        totalFrame = buff;
        data.resize(size_t(totalFrame) * dim.width * dim.height);
        file.read((char*)data.data(), data.size());
    }

    void AniSprite::Paint(AbstractCanvas* canvas, Vec2 coord) const
    {
        const size_t frameOffset =
            size_t(dim.width) * dim.height * playingFrame;
        int i = 0;
        if (coord.y < 0) {
            i = -coord.y;
        }
        for (; i < dim.height; i++) {
            int j = 0;
            if (coord.x < 0) {
                j = -coord.x;
            }
            for (; j < dim.width; j++) {
                (*canvas)[i + coord.y][j + coord.x] =
                    data[frameOffset + i * dim.width + j];
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
        timePassed += deltaTime;
        playingFrame = timePassed * 1000 / frameDuration.count();
        if (playingFrame >= totalFrame) {
            ResetFrame();
            if (!repeat) {
                Stop();
            }
        }
    }
}  // namespace ConsoleGame
