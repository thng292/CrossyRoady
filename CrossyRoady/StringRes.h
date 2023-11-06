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
        std::string_view Title = "Statistic";
        std::string_view PlayTime = "Play Time";
        std::string_view Deaths = "Deaths";
        std::string_view CharUnlocked = "Character Unlocked";
        std::string_view MapUnlocked = "Map Unlocked";
        std::string_view Completion = "Completed";
    } Statistic;

    struct {
        std::string_view Title = "Exp";
    } Exp;

    struct {
        std::string_view Title = "Characters";
    } CharInfo;

    struct {
        std::string_view PlayTime = "PlayTime";
        std::string_view Deaths = "Deaths";
        std::string_view CharUnlocked = "CharUnlocked";
        std::string_view MapUnlocked = "MapUnlocked";
        std::string_view MusicToggle = "Music";
        std::string_view SfxToggle = "Sfx";
        std::string_view OnOpt = "On";
        std::string_view OffOpt = "Off";
    } Config;
};

extern StringResource R;
