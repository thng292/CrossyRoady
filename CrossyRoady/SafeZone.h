#pragma once
#include "Lane.h"

class SafeZone : public Lane {
   public:
    SafeZone() = default;

    SafeZone(double Y, Mob mob0, bool Direc, double Velocity)
    {
        y = Y;
        mob = mob0;
        direc = Direc;
        velocity = Velocity;
    };

    std::string className() { return "SafeZone"; }

    void init();

    void createEntity();

    void deleteEntity(){};

    void updateCoord(){};
};
