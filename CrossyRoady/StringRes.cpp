#include "StringRes.h"

#include <fstream>

Resource R{};

void Config::Load(std::filesystem::path path)
{
    std::ifstream in(path, std::ios::binary | std::ios::in);
    if (in.fail()) {
        return;
    }

    in.read((char*)this, sizeof(*this));
}

void Config::Save(std::filesystem::path path)
{
    std::ofstream out(path, std::ios::binary | std::ios::out);
    if (out.fail()) {
        std::filesystem::create_directories(path.remove_filename());
        out.open(path, std::ios::out | std::ios::binary);
    }

    out.write((char*)this, sizeof(*this));
}
