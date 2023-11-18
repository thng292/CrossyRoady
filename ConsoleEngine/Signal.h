#pragma once

#include <mutex>

namespace ConsoleGame {
    class Signal {
        std::atomic_bool jobRunning = false;
        std::mutex lock;
        std::mutex jobLock;
        std::condition_variable cv;
        std::condition_variable jobCv;

       public:
        Signal() = default;
        ~Signal();

        bool JobDone();

        void StartJob();

        void WaitStartJobSignal();

        void DoneJob();

        void WaitUntilJobDone();
    };

}  // namespace ConsoleGame
