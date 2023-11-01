#pragma once

#include <array>
#include <string_view>

enum class StrRes : uint32_t {
    SinglePlayer,
};

class StringRes {
    static constexpr std::array<const std::string_view, 1> resource = {
        "Single Player",
    };

   public:
    static constexpr auto Get(StrRes res) -> const std::string_view&
    {
        return resource[(uint32_t)res];
    };
};
