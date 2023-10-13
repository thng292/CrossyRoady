#include "Palette.h"

#include <fstream>

namespace ConsoleGame {

    Palette::Palette(std::filesystem::path path) { Load(path); }

    void Palette::Load(std::filesystem::path path)
    {
        filePath = path;
        std::ifstream in(path, std::ios::in | std::ios::binary);
        for (auto& color : data) {
            in.read((char*)&color, 3);
        }
    }

    void Palette::LoadDefault() { data = _DefaultColorPalette; }

    COLORREF Palette::operator[](size_t index) const { return data[index]; }

    const Palette::ColorPalette_t& Palette::GetColorPalette() const { return data; }

    CONSOLE_SCREEN_BUFFER_INFOEX prepareChangeColorPalette()
    {
        CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo;
        bufferInfo.cbSize = sizeof(bufferInfo);
        GetConsoleScreenBufferInfoEx(
            GetStdHandle(STD_INPUT_HANDLE), &bufferInfo
        );
        return bufferInfo;
    }

    void ChangeColorPalette(const Palette& palette)
    {
        static CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo = prepareChangeColorPalette();
        static HANDLE hStdOut = GetStdHandle(STD_INPUT_HANDLE);
        for (int i = 0; i < Palette::ColorPaletteSize; i++) {
            bufferInfo.ColorTable[i] = palette[i];
        }
        SetConsoleScreenBufferInfoEx(hStdOut, &bufferInfo);
    }
}  // namespace ConsoleGame
