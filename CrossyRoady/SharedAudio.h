#pragma once
#include "ConsoleGame.h"

enum class BGMusic : uint8_t {
    Menu,
    Casino,
    City,
    Desert,
    Forest,
    House,
    Space
};

class SharedAudio {
    ConsoleGame::Audio bgMusic;
    ConsoleGame::Audio hoverSound;
    ConsoleGame::Audio clickSound;
    BGMusic current;

    SharedAudio();
   public:
    static SharedAudio& GetInstance();
    void SwitchMusic(BGMusic music);
    void PauseMusic();
    void PlayMusic();
    void ResumeMusic();
    void PlayHoverSfx();
    void PlayClickSfx();
    void UpdateMusicState();
};
