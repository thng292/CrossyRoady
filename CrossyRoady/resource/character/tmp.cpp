#include <bit>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

template <class T>
T HostToBigEndian(T num)
{
    if constexpr (std::endian::native == std::endian::big) {
        return num;
    } else {
        union {
            uint8_t bytes_rep[sizeof(T)];
            T num_rep;
        } tmp{};

        tmp.num_rep = num;
        for (int i = 0, j = sizeof(T) - 1; i < j; i++, j--) {
            uint8_t ttt = tmp.bytes_rep[i];
            tmp.bytes_rep[i] = tmp.bytes_rep[j];
            tmp.bytes_rep[j] = ttt;
        }
        return tmp.num_rep;
    }
}

template <class T>
T BigEndianToHost(T num)
{
    return HostToBigEndian(num);
}

auto main() -> int
{
    constexpr size_t headerSize = 9;
    std::vector<uint8_t> data;
    auto currdir = std::filesystem::directory_iterator(".");
    for (const auto& entry : currdir) {
        if (entry.is_regular_file() &&
            entry.path().extension() == ".anisprite") {
            std::cout << "Updating " << entry.path().filename() << std::endl;
            {
                std::ifstream in(entry.path(), std::ios::binary | std::ios::in);
                uint16_t buff = 0;
                in.read((char*)&buff, sizeof(buff));
                auto width = BigEndianToHost(buff);
                in.read((char*)&buff, sizeof(buff));
                auto height = BigEndianToHost(buff);
                uint32_t b2 = 0;
                in.read((char*)&b2, sizeof(b2));
                auto totalframe = BigEndianToHost(b2);

                in.seekg(0);
                data.resize((size_t)width * height * totalframe + headerSize);
                in.read((char*)data.data(), data.size());
            }
            data[8] = 5;
            {
                std::ofstream out(
                    entry.path(), std::ios::binary | std::ios::out
                );
                out.write((char*)data.data(), data.size());
            }
        }
    }
    return 0;
}
