#include "LocalStorage.h"

#include <fstream>

namespace ConsoleGame {
    LocalStorage::ConfigData_t LocalStorage::data{};

    void LocalStorage::LoadFromFile(std::filesystem::path path)
    {
        std::ifstream in(path, std::ios::in);
        if (in.fail()) {
            std::filesystem::create_directories(path.remove_filename());
            in.open(path, std::ios::in);
        }
        std::string buff;
        while (1) {
            std::getline(in, buff);
            if (in.eof() || in.fail()) {
                break;
            }
            size_t eqPos = buff.find('=');
            if (eqPos == std::string::npos) {
                continue;
            }
            data[buff.substr(0, eqPos)] = buff.substr(eqPos + 1);
        }
    }

    void LocalStorage::SaveToFile(std::filesystem::path path)
    {
        std::ofstream out(path, std::ios::out);
        if (out.fail()) {
            std::filesystem::create_directories(path.remove_filename());
            out.open(path, std::ios::out);
        }

        for (const auto& iVal : data) {
            out << iVal.first << "=" << iVal.second << std::endl;
        }
    }

    const std::string& LocalStorage::Get(const std::string_view config)
    {
        return data[config.data()];
    }

    void LocalStorage::Set(
        const std::string_view config, const std::string& value
    )
    {
        data[config.data()] = value;
    }
}  // namespace ConsoleGame
