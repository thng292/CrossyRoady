#pragma once
#include <deque>

#include "ConsoleGame.h"
#include "Mob.h"

class Lane {
   protected:
    double y;
    std::deque<double> listMob;
    Mob mob;
    bool direc;
    double velocity = 1.0;

   public:
    Lane() = default;
    Lane(double Y, Mob mob0, bool Direc, double Velocity)
        : y(Y), mob(mob0), direc(Direc), velocity(Velocity){};

    virtual double getY() const { return y; };

    virtual std::deque<double> getListMob() const { return listMob; };

    virtual Mob getMob() { return mob; };

    virtual bool getDirec() { return direc; }

    virtual double getVelocity() { return velocity; }

    virtual void setVelocity(double Velocity) { velocity = Velocity; }

    virtual void setY(double Y) { y = Y; };

    virtual void setListMob(std::deque<double> ListMob) { listMob = ListMob; };

    virtual void setMob(Mob mob0) { mob = mob0; };

    virtual void setDirec(bool Direc) { direc = Direc; }

    virtual std::string className() = 0;

    virtual void init();

    virtual void createEntity();

    virtual void deleteEntity();

    virtual void updateCoord();
};
