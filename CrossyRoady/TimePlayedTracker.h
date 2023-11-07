#pragma once
#include <chrono>

class TimePlayedTracker {
    static std::chrono::seconds timePlayed;
    static std::chrono::steady_clock::time_point sessionStart;

   public:
    static void Load();
    static std::chrono::seconds GetTimePlayed();
    static void StartCount();
    static void StopCount();
    static void Save();
};
