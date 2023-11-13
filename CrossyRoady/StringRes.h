#pragma once

#include <array>
#include <filesystem>
#include <string_view>

#include "../ConsoleEngine/Common.h"

#define RESOURCE_PATH "resource/"
#define CHARACTER_PATH "character/"
#define FONT_PATH "font/"
#define MAP_PATH "map/"
#define EXTRA_PATH "extra/"
#define BGM_PATH "bgm/"
#define SFX_PATH "sfx/"

constexpr int LevelExpReq = 200;
constexpr uint8_t numberOfChars = 6;

constexpr std::array<std::string_view, 6> fileCharName = {
    "fauna", "irys", "mumei", "kronii", "sana", "bae"};

constexpr std::array<std::string_view, 6> fileMapName = {
    "forest", "city", "house", "desert", "space", "casino"};

struct CharStuff {
    std::string_view Name;
    std::string_view Skill;
    std::string_view Desc;
};

struct StringResource {
    std::string_view Back = "Back";

    struct {
        std::string_view Exit = "Exit";
    } MainMenu;

    struct {
        std::string_view Title = "Single Player";
    } SinglePlayer;

    struct {
        std::string_view Title = "Multi Player";
    } MultiPlayer;

    struct {
        std::string_view Title = "Progress";
        std::string_view Stat_Title = "Stats";
        std::string_view PlayTime = "Play Time";
        std::string_view Deaths = "Deaths";
        std::string_view Walked = "Walked";
        std::string_view EarnedXP = "Earned XP";
        std::string_view CharUnlocked = "Character Unlocked";
        std::string_view CharUpgraded = "Character Upgraded";
        std::string_view MapUnlocked = "Map Unlocked";
        std::string_view Completion = "Completed";
        std::string_view Exp_Title = "Exp";
        std::string_view Level = "Level";
        std::string_view Rewards = "Rewards";
        std::string_view UnlockNewMap = "Unlock a new map";
        std::string_view UnlockUpgradeToken = "An upgrade token";
        std::string_view Unlock2UpgradeToken = "Two upgrade token";

    } Progress;

    struct {
        std::string_view Title = "Setting";
        std::string_view Music = "Music";
        std::string_view Sfx = "Sfx";
        std::string_view OnOpt = "On";
        std::string_view OffOpt = "Off";
    } Setting;

    struct {
        std::string_view Title = "How to play";
        std::string_view Control = "Control";
        std::string_view Exp = "EXP";
        std::string_view Upgrade = "Upgrade";
    } HowToPlay;

    struct {
        std::string_view Title = "Credit";
    } Credit;

    struct {
        std::string_view Title = "Characters";
        std::string_view UpgradeAvail = "Upgrade Available";
        std::string_view Upgraded = "Upgraded";
        std::string_view NoSkillPoint = "Not enough upgrade point";
        std::string_view Upgrade = "Upgrade";
        std::string_view Skill = "Skill:";
        std::string_view Status = "Status:";
        std::string_view UpgradePoint = "UP: ";
    } CharInfo;

    struct {
        struct {
            std::string_view Name = "Forest";
            std::string_view Debuff =
                "Removes one health whenever the character stops moving";
        } Forest;

        struct {
            std::string_view Name = "City";
            std::string_view Debuff =
                "Decreses the character's health to only one";
        } City;

        struct {
            std::string_view Name = "Haunted House";
            std::string_view Debuff = "Darkens the area around the character";
        } House;

        struct {
            std::string_view Name = "Dessert";
            std::string_view Debuff = "Paralyzes the character's controls";
        } Dessert;

        struct {
            std::string_view Name = "Space";
            std::string_view Debuff =
                "Locks the character's skill until a threshold is passed";
        } Space;

        struct {
            std::string_view Name = "Casino";
            std::string_view Debuff =
                "Randomly chooses a debuff from the other maps";
        } Casino;
    } Map;

    struct {
        CharStuff Fauna = {
            .Name = "Ceres Fauna",
            .Skill = "Gains back full health and with two additional hearts",
            .Desc =
                "The Keeper of Nature, a druidic kirin who materialized in the "
                "mortal realm in a bid to save nature",
        };

        CharStuff Irys = {
            .Name = "Irys",
            .Skill = "Tanks two hits without taking any damage",
            .Desc =
                "A half-demon, half-angel also known as a Nephilim. She has "
                "arrived to deliver hope and determination to the current era",
        };

        CharStuff Mumei = {
            .Name = "Nanashi Mumei",
            .Skill = "Becomes invincible and gains a speed boost",
            .Desc =
                "The Guardian of Civilization, a traveling owl who has borne "
                "witness to numerous events",
        };

        CharStuff Kronii = {
            .Name = "Ouro Kronii",
            .Skill = "Stops every entity's movement",
            .Desc =
                "The Warden of Time, the third concept birthed by the Gods and "
                "the one most intrinsically linked with mankind",
        };

        CharStuff Sana = {
            .Name = "Tsukumo Sana",
            .Skill = "Removes any incoming/active debuff",
            .Desc =
                "The Speaker of Space, a concept unbridled by definition, and "
                "one that continues to grow in scope limitlessly",
        };

        CharStuff Bae = {
            .Name = "Hakos Baelz",
            .Skill = "Randomly chooses any skill, may confuses oneself",
            .Desc =
                "The very concept of Chaos, she believes that rules are not "
                "the be-all and end-all, which is why she has come to break "
                "them all",
        };

    } Character;
};

struct CharStat {
    uint8_t Speed = 0;
    uint8_t Health = 0;
    uint8_t CoolDown = 0;
    uint8_t UpgradedCoolDown = 0;
};

struct CharsStat {
    CharStat Fauna;
    CharStat Irys;
    CharStat Mumei;
    CharStat Kronii;
    CharStat Sana;
    CharStat Bae;
};

struct Config {
    uint32_t PlayTime = 0;
    uint32_t Walked = 0;

    uint16_t Deaths = 0;
    uint16_t CasinoXP = 0;
    uint16_t CityXP = 0;
    uint16_t ForestXP = 0;
    uint16_t HouseXP = 0;
    uint16_t SpaceXP = 0;
    uint16_t DesertXP = 0;

    uint8_t CharUnlocked : 3 = 1;
    uint8_t MapUnlocked : 3 = 1;
    uint8_t Music : 1 = 1;
    uint8_t Sfx : 1 = 1;

    uint8_t FaunaUpgraded : 1 = 0;
    uint8_t IrysUpgraded : 1 = 0;
    uint8_t MumeiUpgraded : 1 = 0;
    uint8_t KroniiUpgraded : 1 = 0;
    uint8_t SanaUpgraded : 1 = 0;
    uint8_t BaeUpgraded : 1 = 0;

    uint8_t UpgradePoint : 3 = 0;

    void Load(std::filesystem::path path);
    void Save(std::filesystem::path path);
    bool GetCharUpgradeStatus(uint8_t character);
    void SetCharUpgradeStatus(uint8_t character);
    uint64_t GetTotalXP();
    uint8_t GetCurrentLevel();
};

struct Resource {
    const StringResource String;
    const CharsStat CharsStat;
    Config Config;
};

extern Resource R;
