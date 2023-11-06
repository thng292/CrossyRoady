#pragma once
#include "Lane.h"

class Road :public Lane{
   public:
    Road() = default;

    Road(double Y, Mob mob0, bool Direc, double Velocity)
        
    {
        y = Y;
        mob=mob0;
        direc = Direc;
        velocity = Velocity;
    };

    std::string className() { return "Road"; }

    void init();

    void createEntity();

    void deleteEntity();

    void updateCoord();
};
