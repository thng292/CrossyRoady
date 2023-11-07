#pragma once
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

namespace ConsoleGame {
    class LocalStorage {
        using ConfigData_t = std::unordered_map<std::string, std::string>;
        static ConfigData_t data;

       public:
        static void LoadFromFile(std::filesystem::path path);
        static void SaveToFile(std::filesystem::path path);
        static const std::string& Get(const std::string_view config);
        static void Set(const std::string_view config, std::string value);
        static void Set(const std::string_view config, std::string_view value);
    };

}  // namespace ConsoleGame
