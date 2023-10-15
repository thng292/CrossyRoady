#include "Font.h"

#include <fstream>

namespace ConsoleGame {
    std::vector<bool> Font::data{};
    Vec2 Font::dim{0};

    void Font::Load(std::filesystem::path path)
    {
        std::fstream in(path, std::ios::in | std::ios::binary);
        uint16_t buff;
        in.read((char*)&buff, sizeof(buff));
        dim.width = BigEndianToHost(buff);
        in.read((char*)&buff, sizeof(buff));
        dim.height = BigEndianToHost(buff);
        const size_t len = size_t(dim.width) * dim.height;
        data.resize(len);

        Color tmp;
        for (size_t i = 0; i < len; i++) {
            in.read((char*)&tmp, sizeof(tmp));
            data[i] = tmp != Color::C_TRANSPARENT;
        }
        dim.height /= charRange;
    }

    void Font::DrawString(
        AbstractCanvas* canvas,
        const std::string_view& str,
        Vec2 coord,
        uint8_t size,
        Color color
    )
    {
        if (coord.x >= _CanvasSize.width || coord.y >= _CanvasSize.height) {
            return;
        }
        if (coord.x + dim.width * str.length() < 0 ||
            coord.y + dim.height < 0) {
            return;
        }

        int nextStop = coord.x - dim.width;
        const int endX = size * dim.width;
        const int endY = size * dim.height;
        const int chSection = dim.width * dim.height;
        for (char ch : str) {
            nextStop += endX;
            if (ch == ' ' || nextStop < 0 || nextStop >= _CanvasSize.width) {
                continue;
            }
            ch -= minCh;
            for (int i = 0; i < endY; i += size) {
                for (int j = 0; j < endX; j += size) {
                    if (data
                            [ch * chSection + (i / size) * dim.width +
                             (j / size)] == true) {
                        for (int m = 0; m < size; m++) {
                            for (int n = 0; n < size; n++) {
                                (*canvas)[i + coord.y + m][j + nextStop + n] =
                                    color;
                            }
                        }
                    }
                }
            }
        }
    }
}  // namespace ConsoleGame
