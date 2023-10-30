#pragma once
#include <deque>

#include "ConsoleGame.h"
#include "Log.h"

class River {
    int y;
    int height;
    std::deque<int> listLog;
    Log log;
    bool direc;
    int velocity;  // Unit: (1/600)/frequency  (pixel/s)
   public:
    River(int Y, int Height, Log log0, bool Direc)
        : y(Y), height(Height), log(log0), direc(Direc){};

    int getY() { return y; };

    int getHeight() { return height; };

    std::deque<int> getListLog()
    {
        std::deque<int> newList = listLog;
        for (int i = 0; i < newList.size(); i++) {
            newList[i] /= 600;
        }
        return newList;
    };

    Log getLog() { return log; };

    bool getDirec() { return direc; }

    int getVelocity() { return velocity; }

    void setVelocity(int Velocity) { velocity = Velocity; }

    void setY(int Y) { y = Y; };

    void setHeight(int Height) { height = Height; };

    void setListLog(std::deque<int> ListLog)
    {
        for (int i = 0; i < ListLog.size(); i++) {
            ListLog[i] *= 600;
        }

        listLog = ListLog;
    };

    void setLog(Log log0) { log = log0; };

    void setDirec(bool Direc) { direc = Direc; }

    std::string className() { return "River"; }

    void Init();

    void createEntity();

    void deleteEntity();

    void updateCoord();
};
