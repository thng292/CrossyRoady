#include "TimePlayedTracker.h"

#include "ConsoleGame.h"
#include "StringRes.h"

using namespace std::literals;
using namespace ConsoleGame;

std::chrono::seconds TimePlayedTracker::timePlayed = 0s;
std::chrono::steady_clock::time_point TimePlayedTracker::sessionStart{};

void TimePlayedTracker::Load()
{
    timePlayed = std::chrono::seconds(R.Config.PlayTime);
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
    Save();
}

void TimePlayedTracker::Save()
{
    R.Config.PlayTime = timePlayed.count();
    R.Config.Save(CONFIG_PATH);
}
