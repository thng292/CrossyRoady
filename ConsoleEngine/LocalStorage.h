#pragma once
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

namespace ConsoleGame {
    class LocalStorage {
        using ConfigData_t = std::unordered_map<std::string_view, std::string>;
        static ConfigData_t data;

       public:
        static void LoadFromFile(std::filesystem::path path);
        static void SaveToFile(std::filesystem::path path);
        static std::string Get(const std::string_view& config);
        static void Set(
            const std::string_view& config, const std::string& value
        );
    };

}  // namespace ConsoleGame
