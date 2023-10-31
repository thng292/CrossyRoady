#include "SafeZone.h"

void SafeZone::createEntity()
{
    if (direc) {
        if (listBlock.back() >=  (block.getSize().width - 2)) {
            listBlock.push_back(
                listBlock.back() - (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) +
                                  block.getSize().width) 
            );
        }
    } else {
        if (listBlock.back() <=
             (ConsoleGame::_CONSOLE_WIDTH_ - block.getSize().width + 1)) {
            listBlock.push_back(
                listBlock.back() + (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) +
                                  block.getSize().width) 
            );
        }
    }
}

void SafeZone::Init()
{
    if (direc) {
        listBlock.push_back( (ConsoleGame::_CONSOLE_WIDTH_ - 1));
        while (listBlock.back() >=  (block.getSize().width - 2)) {
            createEntity();
        }
    } else {
        listBlock.push_back(0);
        while (listBlock.back() <=
                (ConsoleGame::_CONSOLE_WIDTH_ - block.getSize().width + 1)) {
            createEntity();
        }
    }
}
