#include "Palette.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "Canvas.h"
#include "Common.h"

namespace ConsoleGame {

    Palette::Palette(std::filesystem::path path) { Load(path); }

    void Palette::Load(std::filesystem::path path)
    {
        std::cout << "Loading palette from " << path << std::endl;
        std::ifstream in(path, std::ios::in);
        uint8_t r, g, b;
        char hex[3] = {0};
        for (auto& color : data) {
            in.read(hex, sizeof(hex) - 1);
            r = std::stoul(hex, 0, 16);
            in.read(hex, sizeof(hex) - 1);
            g = std::stoul(hex, 0, 16);
            in.read(hex, sizeof(hex) - 1);
            b     = std::stoul(hex, 0, 16);
            color = RGB(r, g, b);
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void Palette::LoadDefault() { data = _DefaultColorPalette; }

    Color& Palette::operator[](size_t index) { return data[index]; }

    Color Palette::operator[](size_t index) const { return data[index]; }

    const ColorPalette_t& Palette::GetColorPalette() const { return data; }

    void ChangeColorPalette(const Palette& palette)
    {
        std::copy_n(
            palette.GetColorPalette().begin(),
            ColorPaletteSize,
            currentColorPallete.begin()
        );
    }
}  // namespace ConsoleGame
