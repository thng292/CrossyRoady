#pragma once
#include <deque>

#include "ConsoleGame.h"
#include "Log.h"

class River {
    int y;
    int height;
    std::deque<double> listLog;
    Log log;
    bool direc;
    double velocity;
   public:
    River(int Y, int Height, Log log0, bool Direc, double Velocity)
        : y(Y), height(Height), log(log0), direc(Direc), velocity(Velocity){};

    int getY() { return y; };

    int getHeight() { return height; };

    std::deque<double> getListLog()
    {
        std::deque<double> newList = listLog;
        for (int i = 0; i < newList.size(); i++) {
            newList[i] /= 600;
        }
        return newList;
    };

    Log getLog() { return log; };

    bool getDirec() { return direc; }

    double getVelocity() { return velocity; }

    void setVelocity(double Velocity) { velocity = Velocity; }

    void setY(int Y) { y = Y; };

    void setHeight(int Height) { height = Height; };

    void setListLog(std::deque<double> ListLog)
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
