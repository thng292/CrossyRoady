#include "Font.h"

#include <fstream>

namespace ConsoleGame {
    std::vector<std::vector<bool>> Font::data{};
    std::vector<Vec2> Font::dim{0};

    Vec2 Font::GetDim(uint8_t variant) { return dim[variant]; }

    void Font::Load(std::filesystem::path path, uint8_t variant)
    {
        if (variant >= data.size()) {
            data.resize((uint16_t)variant + 1);
            dim.resize((uint16_t)variant + 1);
        }
        std::fstream in(path, std::ios::in | std::ios::binary);
        uint16_t buff;
        in.read((char*)&buff, sizeof(buff));
        dim[variant].width = BigEndianToHost(buff);
        in.read((char*)&buff, sizeof(buff));
        dim[variant].height = BigEndianToHost(buff);
        const size_t len = size_t(dim[variant].width) * dim[variant].height;
        data[variant].resize(len);

        Color tmp;
        for (size_t i = 0; i < len; i++) {
            in.read((char*)&tmp, sizeof(tmp));
            data[variant][i] = tmp != Color::C_TRANSPARENT;
        }
        dim[variant].height /= charRange;
    }

    void Font::DrawString(
        AbstractCanvas* canvas,
        const std::string_view& str,
        Vec2 coord,
        uint8_t size,
        uint8_t variant,
        Color color
    )
    {
        if (coord.x >= _CanvasSize.width || coord.y >= _CanvasSize.height) {
            return;
        }
        if (coord.x + dim[variant].width * str.length() < 0 ||
            coord.y + dim[variant].height < 0) {
            return;
        }

        int nextStop = coord.x - dim[variant].width;
        const int endX = size * dim[variant].width;
        const int endY = size * dim[variant].height;
        const int chSection = dim[variant].width * dim[variant].height;
        for (char ch : str) {
            nextStop += endX;
            if (ch == ' ' || nextStop < 0 || nextStop >= _CanvasSize.width) {
                continue;
            }
            ch -= minCh;
            for (int i = 0; i < endY; i += size) {
                for (int j = 0; j < endX; j += size) {
                    if (data[variant]
                            [ch * chSection + (i / size) * dim[variant].width +
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
