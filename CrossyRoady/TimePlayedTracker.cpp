#include "TimePlayedTracker.h"

#include "ConsoleGame.h"
#include "StringRes.h"

using namespace std::literals;
using namespace ConsoleGame;

std::chrono::seconds TimePlayedTracker::timePlayed = 0s;
std::chrono::steady_clock::time_point TimePlayedTracker::sessionStart{};

void TimePlayedTracker::Load()
{
    auto tmp = LocalStorage::Get(R.Config.PlayTime);
    try {
        timePlayed = std::chrono::seconds(std::stoull(tmp));
    } catch (...) {
        LocalStorage::Set(R.Config.PlayTime, "0"s);
    }
}

std::chrono::seconds TimePlayedTracker::GetTimePlayed() { return timePlayed; }

void TimePlayedTracker::StartCount()
{
    sessionStart = std::chrono::steady_clock::now();
}

void TimePlayedTracker::StopCount()
{
    auto tmp = std::chrono::steady_clock::now() - sessionStart;
    constexpr auto secToNano = std::chrono::nanoseconds(1s).count();
    timePlayed += std::chrono::seconds(tmp.count() / secToNano);
}

void TimePlayedTracker::Save()
{
    LocalStorage::Set(
        R.Config.PlayTime, std::format("{}", timePlayed)
    );
}
