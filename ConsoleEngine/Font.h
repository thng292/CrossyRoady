#pragma once
#include <filesystem>
#include <vector>

#include "Abstract/AbstractCanvas.h"
#include "Common.h"

namespace ConsoleGame {
    class Font {
        static std::vector<std::vector<bool>> data;
        static std::vector<Vec2> dim;
        // Font contain symbols from ascii 33 -> 126
        static constexpr uint8_t charRange = 126 - 33 + 1;
        static constexpr uint8_t minCh = 33;
        static constexpr uint8_t maxCh = 126;

       public:
        Font() = delete;
        static Vec2 GetDim(uint8_t variant);
        static void Load(std::filesystem::path path, uint8_t variant = 0);
        static void DrawString(
            AbstractCanvas* canvas,
            const std::string_view& str,
            Vec2 coord,
            uint8_t size = 1,
            uint8_t variant = 0,
            Color color = Color::BLACK
        );
    };

}  // namespace ConsoleGame
