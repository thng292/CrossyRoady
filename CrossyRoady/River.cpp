#include "River.h"

void River::createEntity()
{
    if (direc) {
        if (listLog.back() >=log.getSize().width - 2) {
            listLog.push_back(
                listLog.back() -
                (rand() %
                 abs(ConsoleGame::_CONSOLE_WIDTH_ / 2 - log.getSize().width))
            );
        }
    } else {
        if (listLog.back() <=
            ConsoleGame::_CONSOLE_WIDTH_ - log.getSize().width + 1) {
            listLog.push_back(
                listLog.back() +
                (rand() %
                 abs(ConsoleGame::_CONSOLE_WIDTH_ / 2 - log.getSize().width))
            );
        }
    }
}

void River::Init()
{
    if (direc) {
        listLog.push_back(ConsoleGame::_CONSOLE_WIDTH_ - 1);
        while (listLog.back() >= log.getSize().width - 2) {
            createEntity();
        }
    } else {
        listLog.push_back(0);
        while (listLog.back() <=
               ConsoleGame::_CONSOLE_WIDTH_ - log.getSize().width + 1) {
            createEntity();
        }
    }
