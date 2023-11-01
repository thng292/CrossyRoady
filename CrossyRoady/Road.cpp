#include "Road.h"

void Road::Init()
{
    if (direc) {
        listMob.push_back((ConsoleGame::_CONSOLE_WIDTH_ - 1));
        while (listMob.back() >= (mob.getSize().width - 2)) {
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

void Road::createEntity()
{
    if (direc) {
        if (listMob.back() >= (mob.getSize().width - 2)) {
            double tmp = rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2);
            listMob.push_back(listMob.back() - tmp + mob.getSize().width);
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

void Road::deleteEntity()
{
    if (direc) {
        if (listMob.front() >=
            (ConsoleGame::_CONSOLE_WIDTH_ - 1 + mob.getSize().width)) {
            listMob.pop_front();
        }
    } else {
        if (listMob.front() <= (0 - mob.getSize().width)) {
            listMob.pop_front();
        }
    }
}

void Road::updateCoord()
{
    for (int i = 0; i < listMob.size(); i++) {
        listMob[i] += velocity;
    }
    createEntity();
    deleteEntity();
}
