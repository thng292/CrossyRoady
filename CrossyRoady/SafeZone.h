#pragma once
#include <deque>

#include "Block.h"
#include "ConsoleGame.h"

class SafeZone {
    int y;
    int height;
    std::deque<int> listBlock;
    Block block;
    bool direc;
   public:
    SafeZone(int Y, int Height, Block block0, bool Direc)
        : y(Y), height(Height), block(block0), direc(Direc){};

    int getY() { return y; };

    int getHeight() { return height; };

    std::deque<int> getListBlock()
    {
        std::deque<int> newList = listBlock;
        for (int i = 0; i < newList.size(); i++) {
            newList[i] /= 600;
        }
        return newList;
    };

    Block getBlock() { return block; };

    bool getDirec() { return direc; }

    void setY(int Y) { y = Y; };

    void setHeight(int Height) { height = Height; };

    void setListBlock(std::deque<int> ListBlock) { 
        for (int i = 0; i < ListBlock.size(); i++) {
            ListBlock[i] *= 600;
        }
        listBlock = ListBlock;
    };

    void setBlock(Block block0) { block = block0; };

    void setDirec(bool Direc) { direc = Direc; }

    std::string className() { return "SafeZone"; }

    void createEntity();

    void Init();
};
