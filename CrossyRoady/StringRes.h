#pragma once

#include <array>
#include <filesystem>
#include <string_view>

#define RESOURCE_PATH "resource/"
#define CHARACTER_PATH "character/"
#define FONT_PATH "font/"
#define MAP_PATH "map/"
#define EXTRA_PATH "extra/"
#define BGM_PATH "bgm/"
#define SFX_PATH "sfx/"

constexpr int LevelExpReq = 800;
constexpr uint8_t LEVEL_CAP = 10;
constexpr uint8_t numberOfChars = 6;
constexpr uint8_t numberOfMaps = 6;

constexpr std::array<const std::string_view, numberOfChars> fileCharName = {
    "fauna", "irys", "mumei", "kronii", "sana", "bae"};

constexpr std::array<const std::string_view, numberOfMaps> fileMapName = {
    "forest", "city", "house", "desert", "space", "casino"};

constexpr std::array<const std::string_view, 3> fileMobName = {
    "mob1", "mob2", "mob3"};
constexpr std::array<const std::string_view, numberOfMaps> SongName{
    "Let Me Stay Here",
    "Theme of Irys",
    "Haunted Birdhouse",
    "Daydream",
    "Daystar",
    "Play Dice!"};

constexpr auto CONFIG_PATH = "config.bin";

constexpr auto SAVE_PATH = "save.bin";

struct CharUnlockReq {
    int Auto = 250;
    int Easy = 400;
    int Normal = 300;
    int Hard = 100;
};

struct CharStuff {
    std::string_view Name;
    std::string_view Skill;
    std::string_view Desc;
};

struct MapStuff {
    std::string_view Name;
    std::string_view Debuff;
};

struct StringResource {
    std::string_view Back = "Back";
    std::string_view Next = "Main menu";
    std::string_view Yes = "Yes";
    std::string_view No = "No";

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
        std::string_view Title = "Game Over";
        std::string_view Score = "Score";
        std::string_view DamageTaken = "Damage taken";
        std::string_view PlayTime = "Play time";
        std::string_view MobCollided = "Number of mobs hit";
        std::string_view SkillUse = "Number of skill uses";
        std::string_view ItemPick = "Number of items picked";
        std::string_view DiffReached = "Difficulty";

        std::string_view PlayAgain = "Play again";
        std::string_view MapUnlock = "New map unlocked!";
        std::string_view LevelUp = "Level Up!";
    } Result;

    struct {
        std::string_view ReturnMenu = "Return to main menu";
        std::string_view SaveGame = "Save game?";
        std::string_view Title = "Game paused";
    } Pause;

    struct {
        std::string_view Continue = "Coninue playing your previous save?";
        std::string_view Date = "Date";
        std::string_view PlayTime = "PlayTime";
        std::string_view Score = "Score";
        std::string_view Character = "Character";
        std::string_view Map = "Map";
        std::string_view Difficulty = "Difficulty";
    } AskSave;

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
        std::string_view Skill = "Skill";
        std::string_view Title = "Choose Your Character";
        std::string_view Speed = "Speed: ";
        std::string_view Health = "Health: ";
    } CharSelect;

    struct {
        std::string_view Unlock = "New character unlocked!";
    } CharUnlock;

    struct {
        std::string_view Title = "Choose Level";
        std::string_view Play = "Play";
        std::string_view Mode = "Mode: ";
        std::array<const std::string_view, 4> Modes = {
            "Infinity", "5 min", "10 min", "15 min"};
        std::string_view Difficulty = "Difficulty: ";
        std::array<const std::string_view, 4> Difficulties = {
            "Auto", "Easy", "Normal", "Hard"};
        std::string_view Music = "Music: ";
        std::string_view Debuff = "Debuff: ";
        std::array<const std::string_view, 2> DebuffOpt = {"Off", "On"};
        std::string_view Locked = "Locked";
    } MapSelect;

    struct {
        MapStuff Forest = {
            .Name = "Forest",
            .Debuff = "Removes one health whenever the character stops moving.",
        };

        MapStuff City = {
            .Name = "City",
            .Debuff = "Locks the character's health to only one.",
        };

        MapStuff House = {
            .Name = "Haunted House",
            .Debuff = "Darkens the area around the character.",
        };

        MapStuff Dessert = {
            .Name = "Dessert",
            .Debuff = "Paralyzes the character's movements.",
        };

        MapStuff Space = {
            .Name = "Space",
            .Debuff = "Disables the character's usage of skill.",
        };

        MapStuff Casino = {
            .Name = "Casino",
            .Debuff = "Chooses a random debuff, inverts the controls.",
        };

    } Map;

    struct {
        CharStuff Fauna = {
            .Name = "Ceres Fauna",
            .Skill = "Gains back full health with two additional hearts.",
            .Desc =
                "The Keeper of Nature, a druidic kirin who materialized in the "
                "mortal realm in a bid to save nature.",
        };

        CharStuff Irys = {
            .Name = "Irys",
            .Skill = "Tanks two hits without taking any damage.",
            .Desc =
                "A half-demon, half-angel also known as a Nephilim. She has "
                "arrived to deliver hope and determination to the current era.",
        };

        CharStuff Mumei = {
            .Name = "Nanashi Mumei",
            .Skill = "Becomes invulnerable and gains a speed boost.",
            .Desc =
                "The Guardian of Civilization, a traveling owl who has borne "
                "witness to numerous events.",
        };

        CharStuff Kronii = {
            .Name = "Ouro Kronii",
            .Skill = "Stops every entity's movement.",
            .Desc =
                "The Warden of Time, the third concept birthed by the Gods and "
                "the one most intrinsically linked with mankind.",
        };

        CharStuff Sana = {
            .Name = "Tsukumo Sana",
            .Skill = "Removes any incoming or active debuff.",
            .Desc =
                "The Speaker of Space, a concept unbridled by definition, and "
                "one that continues to grow in scope limitlessly.",
        };

        CharStuff Bae = {
            .Name = "Hakos Baelz",
            .Skill = "Randomly chooses any skill, may confuses oneself.",
            .Desc =
                "The very concept of Chaos, she believes that rules are not "
                "the be-all and end-all, which is why she has come to break "
                "them all.",
        };

    } Character;
};

struct CharStat {
    uint8_t Speed = 0;
    uint8_t Health = 0;
    uint8_t ChargeStep = 0;
};

struct CharsStat {
    CharStat Fauna = {.Speed = 70, .Health = 3, .ChargeStep = 0};
    CharStat Irys = {.Speed = 70, .Health = 4, .ChargeStep = 0};
    CharStat Mumei = {.Speed = 80, .Health = 3, .ChargeStep = 0};
    CharStat Kronii = {.Speed = 60, .Health = 5, .ChargeStep = 0};
    CharStat Sana = {.Speed = 70, .Health = 5, .ChargeStep = 0};
    CharStat Bae = {.Speed = 80, .Health = 3, .ChargeStep = 0};
};

struct DebuffDuration {
    uint8_t Forest = 10;
    uint8_t City = 10;
    uint8_t House = 10;
    uint8_t Desert = 3;
    uint8_t Space = 10;
    uint8_t Casino = 10;
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

    uint8_t MapUnlocked : 3 = 1;
    uint8_t Music : 1 = 1;
    uint8_t Sfx : 1 = 1;

    uint8_t FaunaUnlocked : 1 = 1;
    uint8_t IrysUnlocked : 1 = 0;
    uint8_t MumeiUnlocked : 1 = 0;
    uint8_t KroniiUnlocked : 1 = 0;
    uint8_t SanaUnlocked : 1 = 0;
    uint8_t BaeUnlocked : 1 = 0;

    uint8_t FaunaUpgraded : 1 = 0;
    uint8_t IrysUpgraded : 1 = 0;
    uint8_t MumeiUpgraded : 1 = 0;
    uint8_t KroniiUpgraded : 1 = 0;
    uint8_t SanaUpgraded : 1 = 0;
    uint8_t BaeUpgraded : 1 = 0;

    uint8_t UpgradePoint : 3 = 0;

    uint16_t CasinoHighScore = 0;
    uint16_t CityHighScore = 0;
    uint16_t ForestHighScore = 0;
    uint16_t HouseHighScore = 0;
    uint16_t SpaceHighScore = 0;
    uint16_t DesertHighScore = 0;

    void Load(std::filesystem::path path);
    void Save(std::filesystem::path path);
    bool GetCharUpgradeStatus(uint8_t character);
    uint8_t GetCharUpgraded();
    uint8_t GetCharUnlocked();
    bool GetCharUnlocked(uint8_t character);
    void SetCharUnlocked(uint8_t character);
    void SetCharUpgradeStatus(uint8_t character);
    uint64_t GetTotalXP();
    uint8_t GetCurrentLevel();
};

struct Resource {
    const StringResource String;
    const CharsStat CharsStat;
    const DebuffDuration DebuffDur;
    const CharUnlockReq CharReq;
    Config Config;
};

extern Resource R;
