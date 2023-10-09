#include "Signal.h"

namespace ConsoleGame {

    bool Signal::JobDone() { return jobDone; }

    void Signal::StartJob()
    {
        jobDone = false;
        cv.notify_all();
    }

    void Signal::WaitStartJobSignal()
    {
        cv.wait(ulock, [&] { return jobStarted; });
    }

    void Signal::DoneJob()
    {
        jobDone = true;
        cv.notify_all();
    }

    void Signal::WaitUntilJobDone()
    {
        cv.wait(ulock, [&] { return jobDone; });
    }
}  // namespace ConsoleGame
