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

enum class StrRes : uint32_t {
    SinglePlayer,
    TwoPlayer,
    Progress,
    Setting,
    Exit,
    Back,
    MusicToggle,
    SfxToggle,
    OnOpt,
    OffOpt,

    NameForest,
    NameCity,
    NameHouse,
    NameDesert,
    NameSpace,
    NameCasino,

    DebuffForest,
    DebuffCity,
    DebuffHouse,
    DebuffDesert,
    DebuffSpace,
    DebuffCasino,

    SkillFauna,
    SkillIrys,
    SkillMumei,
    SkillKronii,
    SkillSana,
    SkillBae,

    NameFauna,
    NameIrys,
    NameMumei,
    NameKronii,
    NameSana,
    NameBae,

    DescFauna,
    DescIrys,
    DescMumei,
    DescKronii,
    DescSana,
    DescBae

};

class StringRes {
    static constexpr auto resource{std::to_array<const std::string_view>(
        {"Single Player",
         "Two Player",
         "Progress",
         "Setting",
         "Exit",
         "Back",
         "Music",
         "Sfx",
         "On",
         "Off",

         "The Forest",
         "The City",
         "The Haunted House",
         "The Desert",
         "Space",
         "The Casino",

         "Removes one health whenever the character stops moving",
         "Decreses the character's health to only one",
         "Darkens the area around the character",
         "Paralyzes the character's controls",
         "Locks the character's skill until a threshold is passed",
         "Randomly chooses a debuff from the other maps",

         "Gains back full health and with two additional hearts",
         "Tanks two hits without taking any damage",
         "Becomes invincible and gains a speed boost",
         "Stops every entity's movement",
         "Removes any incoming/active debuff",
         "Randomly chooses any skill, may confuses oneself",

         "Ceres Fauna",
         "Irys",
         "Nanashi Mumei",
         "Ouro Kronii",
         "Tsukumo Sana",
         "Hakos Baelz",

         "The Keeper of Nature, a druidic kirin who materialized in the mortal "
         "realm in a bid to save nature",
         "A half-demon, half-angel also known as a Nephilim. She has arrived "
         "to deliver hope and determination to the current era",
         "The Guardian of Civilization, a traveling owl who has borne witness "
         "to numerous events",
         "The Speaker of Space, a concept unbridled by definition, and one "
         "that continues to grow in scope limitlessly",
         "The Warden of Time, the third concept birthed by the Gods and the "
         "one most intrinsically linked with mankind",
         "The very concept of Chaos, she believes that rules are not the "
         "be-all and end-all, which is why she has come to break them all"

        }
    )};

   public:
    static constexpr auto Get(StrRes res) -> const std::string_view
    {
        return resource[(uint32_t)res];
    };
};
