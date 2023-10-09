#pragma once

#include <mutex>

namespace ConsoleGame {
    class Signal {
        bool jobDone = false;
        bool jobStarted = false;
        std::mutex lock;
        std::unique_lock<std::mutex> ulock;
        std::condition_variable cv;

       public:
        Signal() : ulock(lock){};

        bool JobDone();

        void StartJob();

        void WaitStartJobSignal();

        void DoneJob();

        void WaitUntilJobDone();
    };

}  // namespace ConsoleGame
