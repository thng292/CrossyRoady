#pragma once
#include <deque>

#include "Log.h"
#include "ConsoleGame.h"
class River {
    int y;
    int height;
    std::deque<int> listLog;
    Log log;
    bool direc;

   public:
    River(int Y, int Height, Log log0, bool Direc)
        : y(Y), height(Height), log(log0), direc(Direc){};

    int getY() { return y; };

    int getHeight() { return height; };

    std::deque<int> getListLog() { return listLog; };

    Log getLog() { return log; };

    bool getDirec() { return direc; }

    void setY(int Y) { y = Y; };

    void setHeight(int Height) { height = Height; };

    void setListLog(std::deque<int> ListLog) { listLog = ListLog; };

    void setLog(Log log0) { log = log0; };

    void setDirec(bool Direc) { direc = Direc; }

    std::string className() { return "River"; }

    void createEntity();

    void Init();
};
