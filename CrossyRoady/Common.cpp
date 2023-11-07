#include "Common.h"

#include <format>

std::string SecondsToHMStr(std::chrono::seconds time)
{
    auto min = time.count() / 60;
    auto hour = min / 60;
    min %= 60;
    return std::format("{}h{}", hour, min);
}
