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

uint8_t Config::GetCharUpgraded()
{
    uint8_t res = 0;
    res += R.Config.BaeUpgraded;
    res += R.Config.FaunaUpgraded;
    res += R.Config.IrysUpgraded;
    res += R.Config.KroniiUpgraded;
    res += R.Config.MumeiUpgraded;
    res += R.Config.SanaUpgraded;
    return res;
}

uint8_t Config::GetCharUnlocked()
{
    int res = 0;
    res += FaunaUnlocked;
    res += IrysUnlocked;
    res += MumeiUnlocked;
    res += SanaUnlocked;
    res += KroniiUnlocked;
    res += BaeUnlocked;
    return res;
}

bool Config::GetCharUnlocked(uint8_t character)
{
    switch (character) {
        case 0:
            return FaunaUnlocked;
        case 1:
            return IrysUnlocked;
        case 2:
            return MumeiUnlocked;
        case 3:
            return SanaUnlocked;
        case 4:
            return KroniiUnlocked;
        case 5:
            return BaeUnlocked;
    }
}

void Config::SetCharUnlocked(uint8_t character)
{
    switch (character) {
        case 0:
            FaunaUnlocked = 1;
            break;
        case 1:
            IrysUnlocked = 1;
            break;
        case 2:
            MumeiUnlocked = 1;
            break;
        case 3:
            SanaUnlocked = 1;
            break;
        case 4:
            KroniiUnlocked = 1;
            break;
        case 5:
            BaeUnlocked = 1;
            break;
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

uint8_t Config::GetCurrentLevel() { return std::min(uint8_t(GetTotalXP() / LevelExpReq), LEVEL_CAP); }
