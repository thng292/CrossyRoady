#pragma once

#include <array>
#include <string_view>

#define RESOURCE_PATH "resource/"
#define CHARACTER_PATH "character/"
#define FONT_PATH "font/"
#define MAP_PATH "map/"
#define EXTRA_PATH "extra/"
#define BGM_PATH "bgm/"
#define SFX_PATH "sfx/"

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
    } HowToPlay;

    struct {
        std::string_view Title = "Credit";
    } Credit;

    struct {
        std::string_view Title = "Stats";
        std::string_view PlayTime = "Play Time";
        std::string_view Deaths = "Deaths";
        std::string_view Walked = "Walked";
        std::string_view EarnedXP = "Earned XP";
        std::string_view CharUnlocked = "Character Unlocked";
        std::string_view CharUpgraded = "Character Upgraded";
        std::string_view MapUnlocked = "Map Unlocked";
        std::string_view Completion = "Completed";
    } Statistic;

    struct {
        std::string_view Title = "Exp";
        std::string_view Level = "Level";
        std::string_view Rewards = "Rewards";
        std::string_view UnlockNewMap = "Unlock a new map";
        std::string_view UnlockUpgradeToken = "An upgrade token";
        std::string_view Unlock2UpgradeToken = "Two upgrade token";
    } Exp;

    struct {
        std::string_view Title = "Characters";
        std::string_view UpgradeAvail = "Upgrade Available";
        std::string_view Upgrade = "Upgrade";
    } CharInfo;

    struct {
        std::string_view PlayTime = "PlayTime";
        std::string_view Deaths = "Deaths";
        std::string_view Walked = "Walked";
        std::string_view CharUnlocked = "CharacterUnlocked";
        std::string_view MapUnlocked = "MapUnlocked";
        std::string_view MusicToggle = "Music";
        std::string_view SfxToggle = "Sfx";
        std::string_view OnOpt = "On";
        std::string_view OffOpt = "Off";
        std::string_view True = "True";
        std::string_view False = "False";

        std::string_view BaeUpgraded = "BaeUpgraded";
        std::string_view FaunaUpgraded = "BaeUpgraded";
        std::string_view IrysUpgraded = "BaeUpgraded";
        std::string_view KroniiUpgraded = "BaeUpgraded";
        std::string_view MumeiUpgraded = "BaeUpgraded";
        std::string_view SanaUpgraded = "BaeUpgraded";

        std::string_view CasinoXP = "CasinoXP";
        std::string_view CityXP = "CityXP";
        std::string_view ForestXP = "ForestXP";
        std::string_view HouseXP = "HouseXP";
        std::string_view SpaceXP = "SpaceXP";
        std::string_view DesertXP = "DesertXP";

    } Config;

    struct {
        struct {
            std::string_view Name = "Casino";
            std::string_view Debuff =
                "Randomly chooses a debuff from the other maps";
        } Casino;

        struct {
            std::string_view Name = "City";
            std::string_view Debuff =
                "Decreses the character's health to only one";
        } City;

        struct {
            std::string_view Name = "Dessert";
            std::string_view Debuff = "Paralyzes the character's controls";
        } Dessert;

        struct {
            std::string_view Name = "Forest";
            std::string_view Debuff =
                "Removes one health whenever the character stops moving";
        } Forest;

        struct {
            std::string_view Name = "Haunted House";
            std::string_view Debuff = "Darkens the area around the character";
        } House;

        struct {
            std::string_view Name = "Space";
            std::string_view Debuff =
                "Locks the character's skill until a threshold is passed";
        } Space;
    } Map;

    struct {
        struct {
            std::string_view Name = "Hakos Baelz";
            std::string_view Skill =
                "Randomly chooses any skill, may confuses oneself";
            std::string_view Desc =
                "The very concept of Chaos, she believes that rules are not "
                "the be-all and end-all, which is why she has come to break "
                "them all";
        } Bae;

        struct {
            std::string_view Name = "Tsukumo Sana";
            std::string_view Skill = "Removes any incoming/active debuff";
            std::string_view Desc =
                "The Warden of Time, the third concept birthed by the Gods and "
                "the one most intrinsically linked with mankind";
        } Sana;

        struct {
            std::string_view Name = "Irys";
            std::string_view Skill = "Tanks two hits without taking any damage";
            std::string_view Desc =
                "A half-demon, half-angel also known as a Nephilim. She has "
                "arrived to deliver hope and determination to the current era";
        } Irys;

        struct {
            std::string_view Name = "Ceres Fauna";
            std::string_view Skill =
                "Gains back full health and with two additional hearts";
            std::string_view Desc =
                "The Keeper of Nature, a druidic kirin who materialized in the "
                "mortal realm in a bid to save nature";
        } Fauna;

        struct {
            std::string_view Name = "Nanashi Mumei";
            std::string_view Skill =
                "Becomes invincible and gains a speed boost";
            std::string_view Desc =
                "The Guardian of Civilization, a traveling owl who has borne "
                "witness to numerous events";
        } Mumei;

        struct {
            std::string_view Name = "Ouro Kronii";
            std::string_view Skill = "Stops every entity's movement";
            std::string_view Desc =
                "The Speaker of Space, a concept unbridled by definition, and "
                "one that continues to grow in scope limitlessly";
        } Kronii;

    } Character;
};

extern StringResource R;
