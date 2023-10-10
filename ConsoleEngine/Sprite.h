#pragma once

#include <filesystem>
#include <fstream>

#include "Abstract/AbstractCanvas.h"
#include "Common.h"

namespace ConsoleGame {
    class Sprite {
        std::vector<Color> data;
        Vec2 dim;
        std::filesystem::path openFile;

       public:
        Sprite() = default;
        Sprite(Vec2 dim);
        Sprite(std::filesystem::path filePath);
        Sprite(const Sprite& other);
        Sprite(Sprite&& other) noexcept;
        const Sprite& operator=(const Sprite& other);

        void Load(std::filesystem::path filePath);
        void Clear();
        void Paint(AbstractCanvas* canvas, Vec2 coord) const;
        Vec2 GetDim() const;
        const std::vector<Color>& GetData() const;
        Color* operator[](size_t index);
        void DeInit();
    };
}  // namespace ConsoleGame
