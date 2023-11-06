#include "SafeZone.h"

void SafeZone::createEntity()
{
    if (listMob.back() >= (mob.getSize().width - 2)) {
        listMob.push_back(
            listMob.back() -
            (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) + mob.getSize().width)
        );
    }
}

void SafeZone::init()
{
    listMob.push_back((ConsoleGame::_CONSOLE_WIDTH_ - 1));
    while (listMob.back() >= (mob.getSize().width - 2)) {
        createEntity();
    }
}
