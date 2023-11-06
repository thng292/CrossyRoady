#pragma once
#include "Lane.h"

class River:public Lane {
   public:
    River() = default;

    River(double Y, Mob mob0, bool Direc, double Velocity)
    {
        y = Y;
        mob = mob0;
        direc = Direc;
        velocity = Velocity;
    };

    std::string className() { return "River"; }
 
};
