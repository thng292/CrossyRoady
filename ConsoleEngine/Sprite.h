#pragma once

#include <filesystem>
#include <fstream>

#include "Abstract/AbstractCanvas.h"
#include "Common.h"

namespace ConsoleGame {
    class Sprite {
       protected:
        std::vector<Color> data;
        Vec2 dim;
        Box hitBox;

        void findHitBox();

       public:
        Sprite() = default;
        Sprite(Vec2 dim);
        Sprite(std::filesystem::path filePath);

        const Box& GetHitBox() const;
        void EditHitBox(Vec2 coordOffset, Vec2 dimOffset);
        void Load(std::filesystem::path filePath);
        void Unload();
        void Save(std::filesystem::path filePath);
        void Clear();
        void Draw(AbstractCanvas* canvas, Vec2 coord) const;
        Vec2 GetDim() const;
        const std::vector<Color>& GetData() const;
        Color* operator[](size_t index);
    };
}  // namespace ConsoleGame
