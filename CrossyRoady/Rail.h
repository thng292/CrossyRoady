#pragma once
#include "Lane.h"

class Rail:public Lane {

   public:
    Rail() = default;
    Rail(int Y, Mob mob0, bool Direc, double Velocity)
    {
        y = Y;
        mob = mob0;
        direc = Direc;
        velocity = Velocity;
    };
    std::string className() { return "Rail"; }

    void Init();

    void createEntity();

    void deleteEntity();

    void updateCoord();
};
