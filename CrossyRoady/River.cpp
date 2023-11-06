#include "River.h"

void River::init()
{
    if (direc) {
        listMob.push_back( (ConsoleGame::_CONSOLE_WIDTH_ - 1));
        while (listMob.back() >=  (mob.getSize().width - 2)) {
            createEntity();
        }
    } else {
        listMob.push_back(0);
        while (listMob.back() <=
                (ConsoleGame::_CONSOLE_WIDTH_ - mob.getSize().width + 1)) {
            createEntity();
        }
    }
}

void River::createEntity()
{
    if (direc) {
        if (listMob.back() >=  (mob.getSize().width - 2)) {
            listMob.push_back(
                listMob.back() - (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) +
                                  mob.getSize().width)
            );
        }
    } else {
        if (listMob.back() <=
             (ConsoleGame::_CONSOLE_WIDTH_ - mob.getSize().width + 1)) {
            listMob.push_back(
                listMob.back() + (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) +
                                  mob.getSize().width)
            );
        }
    }
}

void River::deleteEntity()
{
    if (direc) {
        if (listMob.front() >=
             (ConsoleGame::_CONSOLE_WIDTH_ - 1 + mob.getSize().width)) {
            listMob.pop_front();
        }
    } else {
        if (listMob.front() <=  (0 - mob.getSize().width)) {
            listMob.pop_front();
        }
    }
}

void River::updateCoord()
{
    for (int i = 0; i < listMob.size(); i++) {
        listMob[i] += velocity;
    }
    createEntity();
    deleteEntity();
}
