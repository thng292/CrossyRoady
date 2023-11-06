#pragma once
#include "Road.h"
#include "River.h"
#include "Rail.h"
#include "SafeZone.h"
#include <variant>
class Map {
    struct {
        Mob mobRoad, mobRail, mobRiver, mobSafeZone;  
        double vRoad, vRail, vRiver, vSafeZone;
    } Mob_V_Of_Lane;
    std::vector <Lane*> listLane;
    double scores=0;
    int velocity;
    
   public:
    ~Map();
    int numOfRoadOnScr = floor((double)ConsoleGame::_CONSOLE_HEIGHT_ / 32);
    void popFrontList();
    void init();
    void update();
};
