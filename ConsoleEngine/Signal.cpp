#include "Signal.h"

namespace ConsoleGame {

    bool Signal::JobDone() { return !jobRunning; }

    void Signal::StartJob()
    {
        jobRunning = true;
        cv.notify_all();
    }

    void Signal::WaitStartJobSignal()
    {
        std::unique_lock ulock(lock);
        cv.wait(ulock, [&] { return jobRunning.load(); });
    }

    void Signal::DoneJob()
    {
        jobRunning = false;
        jobCv.notify_all();
    }

    void Signal::WaitUntilJobDone()
    {
        std::unique_lock ulock(jobLock);
        jobCv.wait(ulock, [&] { return !jobRunning.load(); });
    }
}  // namespace ConsoleGame
