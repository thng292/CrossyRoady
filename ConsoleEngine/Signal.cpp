#include "Signal.h"

namespace ConsoleGame {

    bool Signal::JobDone() { return jobDone; }

    void Signal::StartJob()
    {
        jobDone = false;
        jobStarted = true;
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
        if (jobStarted) {
            cv.wait(ulock, [&] { return jobDone; });
        }
    }
}  // namespace ConsoleGame
