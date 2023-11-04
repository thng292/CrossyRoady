#pragma once

#include <array>
#include <string_view>

#define RESOURCE_PATH "resource/"
#define CHARACTER_PATH "character/"
#define FONT_PATH "font/"
#define MAP_PATH "map/"
#define EXTRA_PATH "extra/"

enum class StrRes : uint32_t {
    SinglePlayer,
    TwoPlayer,
    Progress,
    Setting,
    Exit,
    Back,
    MusicToggle,
    SfxToggle,
    On,
    Off,
};

class StringRes {
    static constexpr auto resource{std::to_array<const std::string_view>({
        "Single Player",
        "Two Player",
        "Progress",
        "Setting",
        "Exit",
        "Back",
        "Music",
        "Sfx",
        "On",
        "Off",
    })};

   public:
    static constexpr auto Get(StrRes res) -> const std::string_view&
    {
        return resource[(uint32_t)res];
    };
};
