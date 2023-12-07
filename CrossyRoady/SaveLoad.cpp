#include <fstream>

#include "GameMap.h"
#include "SaveLoadType.h"
#include "SharedAudio.h"

using namespace GameType;
using namespace GameMaster;
using namespace ConsoleGame;

void GameMap::SaveGameData()
{
    SaveData saveData;
    size_t listSize = laneList.size();
    saveData.laneList.resize(listSize);
    for (size_t i = 0; i < listSize; ++i) {
        saveData.laneList[i].type = laneList[i]->GetType();
        saveData.laneList[i].IsLeftToRight = laneList[i]->GetIsLeftToRight();
        saveData.laneList[i].hasItem = laneList[i]->GetHasItem();
        saveData.laneList[i].speed = laneList[i]->GetSpeed();
        saveData.laneList[i].laneY = laneList[i]->GetY();

        auto laneType = laneList[i]->GetType();
        if (laneType == ROAD) {
            auto cast = dynamic_cast<Road *>(laneList[i].get());
            saveData.laneList[i].mobType = cast->GetMobType();
        } else if (laneType == RAIL) {
            auto cast = dynamic_cast<Rail *>(laneList[i].get());
            saveData.laneList[i].mobType = cast->GetMobType();
        }
    }
    saveData.character.curHealth = character.GetCurHealth();
    saveData.character.maxHealth = character.GetMaxHealth();
    saveData.character.speed = character.getSpeed();
    saveData.character.type = character.GetType();
    saveData.character.x = character.GetX();
    saveData.character.y = character.GetY();
    saveData.character.direc = character.GetDirec();

    saveData.mapItem.type = mapItem.GetType();
    saveData.mapItem.x = mapItem.GetX();
    saveData.mapItem.y = mapItem.GetY();

    saveData.gameData = gameData;
    saveData.gameEventArgs = gameEventArgs;
    saveData.gameFlags = gameFlags;
    std::ofstream outfile(SAVE_PATH, std::ios::binary);
    if (!outfile) {
        // Handle error opening the file
        return;
    }

    // Write the data to the file
    std::time_t currentTime = std::time(nullptr);
    outfile.write(reinterpret_cast<const char *>(&currentTime), sizeof(time_t));

    outfile.write(
        reinterpret_cast<const char *>(&saveData.gameData), sizeof(GameMapData)
    );
    outfile.write(
        reinterpret_cast<const char *>(&saveData.character), sizeof(ValChar)
    );
    outfile.write(
        reinterpret_cast<const char *>(&saveData.mapItem), sizeof(ValItem)
    );
    outfile.write(
        reinterpret_cast<const char *>(&saveData.gameEventArgs),
        sizeof(GameEventsArgs)
    );
    outfile.write(
        reinterpret_cast<const char *>(&saveData.gameFlags), sizeof(GameFlags)
    );

    outfile.write(reinterpret_cast<const char *>(&listSize), sizeof(listSize));
    for (size_t i = 0; i < listSize; ++i) {
        std::vector<float> curList = laneList[i]->GetEntityList();
        size_t curSize = curList.size();
        outfile.write(
            reinterpret_cast<const char *>(&saveData.laneList[i]),
            sizeof(ValLane)
        );
        outfile.write(
            reinterpret_cast<const char *>(&curSize), sizeof(curSize)
        );
        outfile.write(
            reinterpret_cast<const char *>(&curList[0]), curSize * sizeof(float)
        );
    }
    // Close the file
    outfile.close();
}

std::unique_ptr<Lane> GameMap::GetEquivLane(
    const ValLane &valLane, const std::vector<float> &enList
)
{
    std::unique_ptr<Lane> lane;
    AniSprite *mobSprite = GetMobSprite(valLane.mobType, valLane.IsLeftToRight);
    switch (valLane.type) {
        case ROAD:
            lane = std::make_unique<Road>(
                valLane.laneY,
                valLane.mobType,
                &gameSprites.roadSprite,
                mobSprite,
                valLane.IsLeftToRight,
                enList
            );
            break;
        case RAIL:
            lane = std::make_unique<Rail>(
                valLane.laneY,
                valLane.mobType,
                &gameSprites.roadSprite,
                mobSprite,
                valLane.IsLeftToRight,
                enList,
                valLane.IsLeftToRight ? &gameSprites.arrowRight
                                      : &gameSprites.arrowLeft,
                &gameAudio.railSfx
            );
            break;
        case SAFE:
            lane = std::make_unique<SafeZone>(
                valLane.laneY,
                &gameSprites.safeSprite,
                &gameSprites.blockSprite,
                valLane.IsLeftToRight,
                false,
                enList
            );
            break;
        case WATER:
            lane = std::make_unique<Water>(
                valLane.laneY,
                &gameSprites.waterSprite,
                &gameSprites.floatSprite,
                valLane.IsLeftToRight,
                enList
            );
            break;
    }
    if (valLane.hasItem) {
        lane->SetHasItem(true);
    }
    lane->SetSpeed(valLane.speed);
    return lane;
}

void GameMap::LoadGameData()
{
    std::ifstream infile(SAVE_PATH, std::ios::binary);
    if (!infile) {
        // Handle error opening the file
        return;
    }

    // Read the data from the file
    std::time_t currentTime = std::time(nullptr);
    infile.read(reinterpret_cast<char *>(&currentTime), sizeof(time_t));

    SaveData saveData;
    infile.read(
        reinterpret_cast<char *>(&saveData.gameData), sizeof(GameMapData)
    );
    infile.read(reinterpret_cast<char *>(&saveData.character), sizeof(ValChar));
    infile.read(reinterpret_cast<char *>(&saveData.mapItem), sizeof(ValItem));
    infile.read(
        reinterpret_cast<char *>(&saveData.gameEventArgs),
        sizeof(GameEventsArgs)
    );
    infile.read(
        reinterpret_cast<char *>(&saveData.gameFlags), sizeof(GameFlags)
    );

    gameData = saveData.gameData;
    gameEventArgs = saveData.gameEventArgs;
    gameFlags = saveData.gameFlags;

    LoadSprites();

    character.InitSave(
        saveData.character.type,
        saveData.character.x,
        saveData.character.y,
        saveData.character.curHealth,
        saveData.character.maxHealth,
        saveData.character.speed,
        saveData.character.direc
    );

    mapItem.Init(
        saveData.mapItem.x,
        saveData.mapItem.y,
        saveData.mapItem.type,
        &GetItemSprite(saveData.mapItem.type)
    );

    size_t listSize;
    infile.read(reinterpret_cast<char *>(&listSize), sizeof(listSize));
    saveData.laneList.resize(listSize);
    for (size_t i = 0; i < listSize; ++i) {
        ValLane curLane;
        infile.read(reinterpret_cast<char *>(&curLane), sizeof(ValLane));
        size_t curSize;
        infile.read(reinterpret_cast<char *>(&curSize), sizeof(size_t));
        std::vector<float> enList(curSize);
        infile.read(
            reinterpret_cast<char *>(&enList[0]), curSize * sizeof(float)
        );
        laneList.push_back(GetEquivLane(curLane, enList));
        if (curLane.hasItem) {
            laneWithItem = laneList.back().get();
        }
    }

    if (gameFlags.isFaunaSkill) {
        gameSprites.skillCur = &gameSprites.skillFauna;
    } else if (gameFlags.isIrysSkill) {
        gameSprites.skillCur = &gameSprites.skillIrys;
    } else if (gameFlags.isMumeiSkill) {
        gameSprites.skillCur = &gameSprites.skillMumei;
    } else if (gameFlags.isKroniiSkill) {
        gameSprites.skillCur = &gameSprites.skillKronii;
    } else if (gameFlags.isSanaSkill) {
        gameSprites.skillCur = &gameSprites.skillSana;
    } else if (gameFlags.isBaeSkill) {
        gameSprites.skillCur = &gameSprites.skillInvert;
    }

    // Close the file
    infile.close();
}