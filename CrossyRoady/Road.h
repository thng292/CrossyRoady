#pragma once
#include <deque>

#include "ConsoleGame.h"
#include "Mob.h"

class Road {
    int y;
    int height;
    std::deque<double> listMob;
    Mob mob;
    bool direc;
    int velocity = 300;  // Unit: (1/600)/frequency  (pixel/s)
    double velocity;  
   public:
    Road(int Y, int Height, Mob mob0, bool Direc)
        : y(Y), height(Height), mob(mob0), direc(Direc){};

    int getY() const { return y; };

    int getHeight() { return height; };

    std::deque<int> getListMob()
    {
        return listMob;
    };

    Mob getMob() { return mob; };

    bool getDirec() { return direc; }

    double getVelocity() { return velocity; }

    void setVelocity(double Velocity) { velocity = Velocity; }

    void setY(int Y) { y = Y; };

    void setHeight(int Height) { height = Height; };

    void setListMob(std::deque<double> ListMob)
    {
        listMob = ListMob;
    };

    void setMob(Mob mob0) { mob = mob0; };

    void setDirec(bool Direc) { direc = Direc; }

    std::string className() { return "Road"; }

    void Init();

    void createEntity();

    void deleteEntity();

    void updateCoord();
};
