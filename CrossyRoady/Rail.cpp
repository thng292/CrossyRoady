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
                    dem - 4 * ConsoleGame::_CONSOLE_WIDTH_ -
                    i * mob.getSize().width
                );
            }
        }
    } else {
        if (listMob.back() < ConsoleGame::_CONSOLE_WIDTH_-1) {
            int dem = listMob.back();
            for (int i = 0;
                 i <= ConsoleGame::_CONSOLE_WIDTH_ / mob.getSize().width;
                 i++) {
                listMob.push_back(
                    dem + 4 * ConsoleGame::_CONSOLE_WIDTH_ +
                    i * mob.getSize().width
                );
            }
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
                ConsoleGame::_CONSOLE_WIDTH_ - 1 - i * mob.getSize().width
            );
        }
        createEntity();
    } else {
        for (int i = 0; i <= ConsoleGame::_CONSOLE_WIDTH_ / mob.getSize().width;
             i++) {
            listMob.push_back(
                i * mob.getSize().width
            );
        }
        createEntity();
    }

}
