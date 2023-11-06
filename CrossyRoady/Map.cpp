#include "Map.h"

Map::~Map()
{
    for (int i = 0; i < listLane.size(); i++) {
        delete listLane[i];
    }
}

void Map::popFrontList()
{ 
    delete listLane[0];
    listLane.erase(listLane.begin()); }

void Map::init( )
{
    scores = 0;
    for (int i = 0; i < 3; i++) {
        listLane.push_back(new SafeZone(
            i*32, mob_V_Of_Lane.mobSafeZone, rand() % 2, mob_V_Of_Lane.vSafeZone
        ));
    }
    

    for (int i = 3; i < 2 * numOfRoadOnScr; i++) {
        switch (rand() % 4) {
            case 0: {
                listLane.push_back(new Road(
                    i*32,
                    mob_V_Of_Lane.mobRoad,
                    rand() % 2,
                    mob_V_Of_Lane.vRoad
                )
                );
            }
            case 1: {
                listLane.push_back(new River(
                    i*32,
                    mob_V_Of_Lane.mobRiver,
                    rand() % 2,
                    mob_V_Of_Lane.vRiver
                ));
            }
            case 2: {
                listLane.push_back(new Rail(
                    i*32,
                    mob_V_Of_Lane.mobRail,
                    rand() % 2,
                    mob_V_Of_Lane.vRail
                )
                );
            }
            default: {
                listLane.push_back(new SafeZone(
                    i*32,
                    mob_V_Of_Lane.mobSafeZone,
                    rand() % 2,
                    mob_V_Of_Lane.vSafeZone
                )
                );
            }

        }
    }
}

void Map::update()
{
    setVelocity();
    for (int i = 0; i < listLane.size(); i++) {
        listLane[i]->setY(listLane[i]->getY() - velocity);
    }
    scores += velocity / 32;
    if (listLane[0]->getY() <= 32) {
        popFrontList();
        switch (rand() % 4) {
            case 0: {
                listLane.push_back(new Road(
                    listLane.back()->getY()+32,
                    mob_V_Of_Lane.mobRoad,
                    rand() % 2,
                    mob_V_Of_Lane.vRoad
                ));
            }
            case 1: {
                listLane.push_back(new River(
                    listLane.back()->getY() + 32,
                    mob_V_Of_Lane.mobRiver,
                    rand() % 2,
                    mob_V_Of_Lane.vRiver
                ));
            }
            case 2: {
                listLane.push_back(new Rail(
                    listLane.back()->getY() + 32,
                    mob_V_Of_Lane.mobRail,
                    rand() % 2,
                    mob_V_Of_Lane.vRail
                ));
            }
            default: {
                listLane.push_back(new SafeZone(
                    listLane.back()->getY() + 32,
                    mob_V_Of_Lane.mobSafeZone,
                    rand() % 2,
                    mob_V_Of_Lane.vSafeZone
                ));
            }
        }
    }
}

void Map::setVelocity()
{
    if (charac.getCoord().y > ConsoleGame::_CONSOLE_HEIGHT_ / 2) {
        velocity = originalVelocity + subVeclocity;
    } else {
        velocity = originalVelocity;
    }
}

