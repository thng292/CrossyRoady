#pragma once
#include <filesystem>
#include <vector>

#include "Abstract/AbstractCanvas.h"
#include "Common.h"

namespace ConsoleGame {
    class Font {
        static std::vector<bool> data;
        static Vec2 dim;
        // Font contain symbols from ascii 33 -> 126
        static constexpr uint8_t charRange = 126 - 33 + 1;
        static constexpr uint8_t minCh = 33;
        static constexpr uint8_t maxCh = 126;

       public:
        Font() = delete;
        static void Load(std::filesystem::path path);
        static void DrawString(
            AbstractCanvas* canvas,
            const std::string_view& str,
            Vec2 coord,
            uint8_t size = 1,
            Color color = Color::BLACK
        );
    };

}  // namespace ConsoleGame
