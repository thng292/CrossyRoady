#include "SharedAudio.h"

#include "StringRes.h"

using namespace ConsoleGame;

SharedAudio::SharedAudio()
{
    current = BGMusic::Menu;
    hoverSound.Open(RESOURCE_PATH SFX_PATH "hover.wav");
    clickSound.Open(RESOURCE_PATH SFX_PATH "select.wav");
    bgMusic.Open(RESOURCE_PATH BGM_PATH "menu.mp3");
}

SharedAudio& SharedAudio::GetInstance()
{
    static SharedAudio instance;
    return instance;
}

void SharedAudio::SwitchMusic(BGMusic music)
{
    using enum BGMusic;
    if (current == music) {
        return;
    } else {
        current = music;
    }
    switch (music) {
        case Menu:
            bgMusic.ChangeSong(RESOURCE_PATH BGM_PATH "menu.mp3");
            break;
        case Casino:
            bgMusic.ChangeSong(RESOURCE_PATH BGM_PATH "casino.mp3");
            break;
        case City:
            bgMusic.ChangeSong(RESOURCE_PATH BGM_PATH "city.mp3");
            break;
        case Desert:
            bgMusic.ChangeSong(RESOURCE_PATH BGM_PATH "desert.mp3");
            break;
        case Forest:
            bgMusic.ChangeSong(RESOURCE_PATH BGM_PATH "forest.mp3");
            break;
        case House:
            bgMusic.ChangeSong(RESOURCE_PATH BGM_PATH "house.mp3");
            break;
        case Space:
            bgMusic.ChangeSong(RESOURCE_PATH BGM_PATH "space.mp3");
            break;
    }
    PlayMusic();
}

void SharedAudio::PauseMusic() { bgMusic.Pause(); }

void SharedAudio::PlayMusic()
{
    if (R.Config.Music and not bgMusic.IsPlaying()) {
        bgMusic.Play(true, true);
    }
}

void SharedAudio::ResumeMusic()
{
    if (R.Config.Music) {
        bgMusic.Resume();
    }
}

void SharedAudio::PlayHoverSfx()
{
    if (R.Config.Sfx and IsWindowForeground()) {
        hoverSound.Play();
    }
}

void SharedAudio::PlayClickSfx()
{
    if (R.Config.Sfx and IsWindowForeground()) {
        clickSound.Play();
    }
}

void SharedAudio::UpdateMusicState()
{
    if (R.Config.Music) {
        bgMusic.Resume();
    } else {
        bgMusic.Pause();
    }
}
