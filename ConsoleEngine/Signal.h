#pragma once

#include <mutex>

namespace ConsoleGame {
    class Signal {
        bool jobRunning = false;
        std::mutex lock;
        std::mutex jobLock;
        std::condition_variable cv;
        std::condition_variable jobCv;

       public:
        Signal() = default;

        bool JobDone();

        void StartJob();

        void WaitStartJobSignal();

        void DoneJob();

        void WaitUntilJobDone();
    };

}  // namespace ConsoleGame
