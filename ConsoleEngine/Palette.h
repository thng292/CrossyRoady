#pragma once

#include <array>
#include <filesystem>

#include "Common.h"

namespace ConsoleGame {
    class Palette {
       public:
        static constexpr ColorPalette_t _DefaultColorPalette = {
            RGB(12, 12, 12),
            RGB(0, 55, 218),
            RGB(19, 161, 14),
            RGB(58, 150, 221),
            RGB(197, 15, 31),
            RGB(136, 23, 152),
            RGB(193, 156, 0),
            RGB(204, 204, 204),
            RGB(118, 118, 118),
            RGB(59, 120, 255),
            RGB(22, 198, 12),
            RGB(97, 214, 214),
            RGB(231, 72, 86),
            RGB(180, 0, 158),
            RGB(249, 241, 165),
            RGB(242, 242, 242),
        };

       private:
        ColorPalette_t data;

       public:
        Palette() = default;
        Palette(std::filesystem::path path);
        void Load(std::filesystem::path path);
        void LoadDefault();
        const ColorPalette_t& GetColorPalette() const;
        Color& operator[](size_t index);
        Color operator[](size_t index) const;

        friend void ChangeColorPalette(const Palette& palette);
    };

};  // namespace ConsoleGame
