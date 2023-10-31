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
    double velocity;  
   public:
    Road(int Y, int Height, Mob mob0, bool Direc,double Velocity)
        : y(Y), height(Height), mob(mob0), direc(Direc),velocity(Velocity){};

    int getY() { return y; };

    int getHeight() { return height; };

    std::deque<double> getListMob()
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
