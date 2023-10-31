#include "River.h"

void River::Init()
{
    if (direc) {
        listLog.push_back( (ConsoleGame::_CONSOLE_WIDTH_ - 1));
        while (listLog.back() >=  (log.getSize().width - 2)) {
            createEntity();
        }
    } else {
        listLog.push_back(0);
        while (listLog.back() <=
                (ConsoleGame::_CONSOLE_WIDTH_ - log.getSize().width + 1)) {
            createEntity();
        }
    }
}

void River::createEntity()
{
    if (direc) {
        if (listLog.back() >=  (log.getSize().width - 2)) {
            listLog.push_back(
                listLog.back() - (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) +
                                  log.getSize().width)
            );
        }
    } else {
        if (listLog.back() <=
             (ConsoleGame::_CONSOLE_WIDTH_ - log.getSize().width + 1)) {
            listLog.push_back(
                listLog.back() + (rand() % (ConsoleGame::_CONSOLE_WIDTH_ / 2) +
                                  log.getSize().width)
            );
        }
    }
}

void River::deleteEntity()
{
    if (direc) {
        if (listLog.front() >=
             (ConsoleGame::_CONSOLE_WIDTH_ - 1 + log.getSize().width)) {
            listLog.pop_front();
        }
    } else {
        if (listLog.front() <=  (0 - log.getSize().width)) {
            listLog.pop_front();
        }
    }
}

void River::updateCoord()
{
    for (int i = 0; i < listLog.size(); i++) {
        listLog[i] += velocity;
    }
    createEntity();
    deleteEntity();
}
