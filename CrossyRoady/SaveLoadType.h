#pragma once
#include "GameMaster.h"
#include "GameType.h"

struct ValLane {
    GameType::LaneType type;
    GameType::MobType mobType;
    bool IsLeftToRight;
    bool hasItem;
    float speed;
    float laneY;
};

struct ValChar {
    GameType::CharaType type;
    GameType::Direction direc;
    int maxHealth;
    int curHealth;
    float speed;
    float x;
    float y;
};

struct ValItem {
    float x, y;
    GameType::ItemType type;
};

struct SaveData {
    std::vector<ValLane> laneList;
    ValChar character;
    ValItem mapItem;
    GameType::GameMapData gameData;
    GameMaster::GameEventsArgs gameEventArgs;
    GameMaster::GameFlags gameFlags;
};