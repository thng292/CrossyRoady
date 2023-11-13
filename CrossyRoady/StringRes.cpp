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

uint64_t Config::GetTotalXP()
{
    uint64_t res = 0;
    res += R.Config.CasinoXP;
    res += R.Config.CityXP;
    res += R.Config.DesertXP;
    res += R.Config.ForestXP;
    res += R.Config.HouseXP;
    res += R.Config.SpaceXP;
    return res;
}

bool Config::GetCharUpgradeStatus(uint8_t character)
{
    switch (character) {
        case 0:
            return FaunaUpgraded;
        case 1:
            return IrysUpgraded;
        case 2:
            return MumeiUpgraded;
        case 3:
            return SanaUpgraded;
        case 4:
            return KroniiUpgraded;
        case 5:
            return BaeUpgraded;
    }
}

void Config::SetCharUpgradeStatus(uint8_t character)
{
    switch (character) {
        case 0:
            FaunaUpgraded = 1;
            break;
        case 1:
            IrysUpgraded = 1;
            break;
        case 2:
            MumeiUpgraded = 1;
            break;
        case 3:
            SanaUpgraded = 1;
            break;
        case 4:
            KroniiUpgraded = 1;
            break;
        case 5:
            BaeUpgraded = 1;
            break;
    }
}

uint8_t Config::GetCurrentLevel() { return GetTotalXP() / LevelExpReq; }
