#include "Palette.h"

#include <fstream>

namespace ConsoleGame {

    Palette::Palette(std::filesystem::path path) { Load(path); }

    void Palette::Load(std::filesystem::path path)
    {
        filePath = path;
        std::ifstream in(path, std::ios::in);
        uint8_t r, g, b;
        char hex[2] = {0};
        for (auto& color : data) {
            in.read(hex, sizeof(hex));
            r = std::stoi(hex, 0, 16);
            in.read(hex, sizeof(hex));
            g = std::stoi(hex, 0, 16);
            in.read(hex, sizeof(hex));
            b = std::stoi(hex, 0, 16);
            color = RGB(r, g, b);
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void Palette::LoadDefault() { data = _DefaultColorPalette; }

    COLORREF Palette::operator[](size_t index) const { return data[index]; }

    const Palette::ColorPalette_t& Palette::GetColorPalette() const
    {
        return data;
    }

    CONSOLE_SCREEN_BUFFER_INFOEX prepareChangeColorPalette()
    {
        CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo;
        bufferInfo.cbSize = sizeof(bufferInfo);
        bool ok = GetConsoleScreenBufferInfoEx(
            GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo
        );
        int tmp;
        if (!ok) {
            tmp = GetLastError();
        }
        bufferInfo.srWindow.Right = _ScreenSize.width;
        bufferInfo.srWindow.Bottom = _ScreenSize.height;
        return bufferInfo;
    }

    void ChangeColorPalette(const Palette& palette)
    {
        static CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo =
            prepareChangeColorPalette();
        static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int i = 0; i < Palette::ColorPaletteSize; i++) {
            bufferInfo.ColorTable[i] = palette[i];
        }
        bool ok = SetConsoleScreenBufferInfoEx(hStdOut, &bufferInfo);
        int tmp;
        if (!ok) {
            tmp = GetLastError();
        }
    }
}  // namespace ConsoleGame
