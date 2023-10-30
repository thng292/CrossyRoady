#include "Rail.h"

void Rail::createEntity()
{
    if (direc) {
        if (listMob.back() > 0 ) {
            int dem = listMob.back();
            for (int i = 0;
                 i <= ConsoleGame::_CONSOLE_WIDTH_ / mob.getSize().width;
                 i++) {
                listMob.push_back(
                    dem - 4 * ConsoleGame::_CONSOLE_WIDTH_*600 -
                    i * mob.getSize().width *600
                );
            }
        }
    } else {
        if (listMob.back() < 600*(ConsoleGame::_CONSOLE_WIDTH_-1)) {
            int dem = listMob.back();
            for (int i = 0;
                 i <= ConsoleGame::_CONSOLE_WIDTH_ / mob.getSize().width;
                 i++) {
                listMob.push_back(
                    dem + 4 * ConsoleGame::_CONSOLE_WIDTH_ *600+
                    i * mob.getSize().width*600
                );
            }
        }
    }
}


void Rail::Init()
{
    if (direc) {
        for (int i = 0; i <= ConsoleGame::_CONSOLE_WIDTH_ / mob.getSize().width;
             i++) {
            listMob.push_back(
                600*(ConsoleGame::_CONSOLE_WIDTH_ - 1 - i * mob.getSize().width)
            );
        }
        createEntity();
    } else {
        for (int i = 0; i <= ConsoleGame::_CONSOLE_WIDTH_ / mob.getSize().width;
             i++) {
            listMob.push_back(
                i * mob.getSize().width*600
            );
        }
        createEntity();
    }

}

void Rail::deleteEntity()
{
    if (direc) {
        if (listMob.front() >=
            600 * (ConsoleGame::_CONSOLE_WIDTH_ *2- 1 +3* mob.getSize().width)) {
            for (int i = 0;
                 i <= ConsoleGame::_CONSOLE_WIDTH_ / mob.getSize().width;
                 i++) {
                listMob.pop_front();
            }
        }
    } else {
        if (listMob.front() <=
            600 * (0 - ConsoleGame::_CONSOLE_WIDTH_- 3*mob.getSize().width)) {
            for (int i = 0;
                 i <= ConsoleGame::_CONSOLE_WIDTH_ / mob.getSize().width;
                 i++) {
                listMob.pop_front();
            }
        }
    }
}

void Rail::updateCoord()
{
    for (int i = 0; i < listMob.size(); i++) {
        listMob[i] += velocity;
    }
    createEntity();
    deleteEntity();
}
