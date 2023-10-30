#pragma once
#include <deque>

#include "ConsoleGame.h"
#include "Mob.h"

class Road {
    int y;
    int height;
    std::deque<int> listMob;
    Mob mob;
    bool direc;         
    int velocity;           // Unit: (1/600)/frequency  (pixel/s)
   public:
    Road(int Y, int Height, Mob mob0, bool Direc)
        : y(Y), height(Height), mob(mob0), direc(Direc){};

    int getY() { return y; };

    int getHeight() { return height; };

    std::deque<int> getListMob() { return listMob; };

    Mob getMob() { return mob; };

    bool getDirec() { return direc; }

    int getVelocity() { return velocity; }

    void setVelocity(int Velocity) { velocity = Velocity; }

    void setY(int Y) { y = Y; };

    void setHeight(int Height) { height = Height; };

    void setListMob(std::deque<int> ListMob) { listMob = ListMob; };

    void setMob(Mob mob0) { mob = mob0; };

    void setDirec(bool Direc) { direc = Direc; }

    std::string className() { return "Road"; }

    void createEntity();

    void Init();

    void updateCoord();
};
