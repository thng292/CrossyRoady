#include "LocalStorage.h"

#include <fstream>

namespace ConsoleGame {
    LocalStorage::ConfigData_t LocalStorage::data{};

    void LocalStorage::LoadFromFile(std::filesystem::path path)
    {
        std::ifstream in(path, std::ios::in);
        std::string buff;
        while (1) {
            std::getline(in, buff);
            if (in.eof()) {
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
        for (const auto& iVal : data) {
            out << iVal.first << "=" << iVal.second << std::endl;
        }
    }

    std::string LocalStorage::Get(const std::string_view& config)
    {
        return data[config];
    }

    void LocalStorage::Set(const std::string_view& config, const std::string& value)
    {
        data[config] = value;
    }
}  // namespace ConsoleGame