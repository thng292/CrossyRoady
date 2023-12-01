#include "MapSelect.h"

#include "GameMap.h"
#include "GameUtils.h"
using namespace ConsoleGame;
constexpr auto BGPrimary = (ConsoleGame::Color)2;
constexpr auto BGSecond = (ConsoleGame::Color)1;
constexpr auto TriColor = (ConsoleGame::Color)0;
const auto* maps = &R.String.Map.Forest;
constexpr int previewWidth = 176;

void MapSelect::ChangePreview()
{
    const auto tmp = Palette(std::format(
        RESOURCE_PATH MAP_PATH "{}/{}.hex",
        fileMapName[userOpt.map],
        fileMapName[userOpt.map]
    ));

    for (int i = 3; i < tmp.GetColorPalette().size(); i++) {
        currentPalette[i] = tmp[i];
    }
    ChangeColorPalette(currentPalette);

    preview.Load(std::format(
        RESOURCE_PATH MAP_PATH "{}/preview.sprite", fileMapName[userOpt.map]
    ));
    debuff.Load(std::format(
        RESOURCE_PATH MAP_PATH "{}/debuff.sprite", fileMapName[userOpt.map]
    ));

    if (not hasChangedMusic) {
        userOpt.music = userOpt.map;
        audio.SwitchMusic(BGMusic(userOpt.music));
        audio.PlayMusic();
    }
    mapTitleButton.ChangeText(maps[userOpt.map].Name);
}

void MapSelect::UpdateStr()
{
    DifficultyTitle = std::format(
        "{}{}",
        R.String.MapSelect.Difficulty,
        R.String.MapSelect.Difficulties[userOpt.difficulty]
    );
    menu.buttons[1].ChangeText(DifficultyTitle);

    ModeTitle = std::format(
        "{}{}", R.String.MapSelect.Mode, R.String.MapSelect.Modes[selectedMode]
    );
    menu.buttons[3].ChangeText(ModeTitle);

    MusicTitle =
        std::format("{}{}", R.String.MapSelect.Music, SongName[userOpt.music]);
    menu.buttons[2].ChangeText(MusicTitle);

    DebuffTitle = std::format(
        "{}{}",
        R.String.MapSelect.Debuff,
        R.String.MapSelect.DebuffOpt[enableDebuff]
    );
    menu.buttons[4].ChangeText(DebuffTitle);
}

const std::wstring_view MapSelect::ScreenName() { return L"MapSelect"; }

std::wstring_view MapSelect::getName() { return ScreenName(); }

void MapSelect::Init(const std::any& args)
{
    userOpt = std::any_cast<GameType::UserOption>(args);
    constexpr Vec2 startPos = {210, 40};
    constexpr Vec2 buttSize = {160, 19};
    userOpt.map = R.Config.MapUnlocked - 1;
    menu.Init(
        startPos,
        buttSize,
        {
            R.String.MapSelect.Play,
            DifficultyTitle,
            MusicTitle,
            ModeTitle,
            DebuffTitle,
            R.String.Back,
        }
    );

    musicMenu.Init(startPos, buttSize, SongName);
    for (int i = R.Config.MapUnlocked; i < numberOfMaps; i++) {
        musicMenu.buttons[i].ChangeText(R.String.MapSelect.Locked);
    }
    diffMenu.Init(startPos, buttSize, R.String.MapSelect.Difficulties);
    modeMenu.Init(startPos, buttSize, R.String.MapSelect.Modes);

    constexpr int arrowPosY = 150 + 5;  // 70
    mapL = ArrowButton(
        SurfaceArgs{
            .pos = {20, arrowPosY},
            .cornerSize = uint8_t(Font::GetDim(1).height / 2 - 1),
            .hasBorder = true,
            .background = (Color)14,
            .border = (Color)13},
        false
    );
    mapR = ArrowButton(
        SurfaceArgs{
            .pos =
                {15 + previewWidth - 5 - Font::GetDim(1).height / 2, arrowPosY},
            .cornerSize = uint8_t(Font::GetDim(1).height / 2 - 1),
            .hasBorder = true,
            .background = (Color)14,
            .border = (Color)13},
        true
    );
    debuffSurface.props = {
        .size = {previewWidth - 1, Font::GetDim(0).height * 2 + 15},
        .pos = {15, 170},
        .hasBorder = true,
        .border = (Color)14};
    mapTitleButton = Button(
        {.size = {previewWidth, 20}, .pos = {15, 150}, .background = (Color)14},
        R.String.MapSelect.Title,
        BGPrimary,
        1,
        1
    );
}

void MapSelect::Mount(const std::any& args)
{
    currentPalette.Load(
        std::format(RESOURCE_PATH EXTRA_PATH "{}-sel.hex", fileCharName[0])
    );
    currentPalette[0] = currentPalette[10];
    currentPalette[1] = currentPalette[11];
    currentPalette[2] = currentPalette[12];
    ChangePreview();
    UpdateStr();
}

AbstractScreen* MapSelect::Clone() const { return new MapSelect; }

AbstractNavigation::NavigationRes MapSelect::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    const auto handleOnHover = [&](uint8_t) noexcept { audio.PlayHoverSfx(); };
    auto res = navigation->NoChange();
    lastSelectedMap = userOpt.map;
    switch (currentMenu) {
        case 0:
            if (userOpt.map != 0) {
                mapL.Update(
                    deltaTime,
                    [&] { audio.PlayHoverSfx(); },
                    [&] {
                        audio.PlayClickSfx();
                        userOpt.map--;
                    }
                );
            }
            if (userOpt.map != R.Config.MapUnlocked - 1) {
                mapR.Update(
                    deltaTime,
                    [&] { audio.PlayHoverSfx(); },
                    [&] {
                        audio.PlayClickSfx();
                        userOpt.map++;
                    }
                );
            }

            menu.Update(
                deltaTime,
                handleOnHover,
                [&](uint8_t choosed) noexcept {
                    audio.PlayClickSfx();
                    switch (choosed) {
                        case 0:
                            res = navigation->Navigate(
                                GameMap::ScreenName(), userOpt
                            );
                            break;
                        case 1:
                        case 2:
                        case 3:
                            currentMenu = choosed;
                            break;
                        case 4:
                            enableDebuff = !enableDebuff;
                            UpdateStr();
                            break;
                        case 5:
                            res = navigation->Back();
                            break;
                    }
                }
            );
            break;
        case 1:
            diffMenu.Update(
                deltaTime,
                handleOnHover,
                [&](uint8_t choosed) noexcept {
                    audio.PlayClickSfx();
                    currentMenu = 0;
                    userOpt.difficulty = choosed;
                    UpdateStr();
                }
            );
            break;
        case 2:
            musicMenu.Update(
                deltaTime,
                handleOnHover,
                [&](uint8_t choosed) noexcept {
                    audio.PlayClickSfx();
                    if (choosed < R.Config.MapUnlocked) {
                        currentMenu = 0;
                        userOpt.music = choosed;
                        hasChangedMusic = true;
                        UpdateStr();
                        audio.SwitchMusic(BGMusic(userOpt.music));
                        audio.PlayMusic();
                    }
                }
            );
            break;
        case 3:
            modeMenu.Update(
                deltaTime,
                handleOnHover,
                [&](uint8_t choosed) noexcept {
                    audio.PlayClickSfx();
                    currentMenu = 0;
                    userOpt.isTimed = !!choosed;
                    userOpt.time = choosed * 300;
                    selectedMode = choosed;
                    UpdateStr();
                }
            );
            break;
    }

    if (lastSelectedMap != userOpt.map) {
        ChangePreview();
        UpdateStr();
    }

    return res;
}

void MapSelect::Draw(AbstractCanvas* canvas) const
{
    canvas->Clear(BGPrimary);
    GameUtils::DrawBLTriangle(canvas, 70, TriColor);
    GameUtils::DrawTRTriangle(canvas, 70, TriColor);

    Font::DrawString(
        canvas,
        R.String.MapSelect.Title,
        {(_CanvasSize.width -
          int(R.String.MapSelect.Title.size()) * Font::GetDim(1).width) /
             2,
         15},
        1,
        1,
        (Color)14
    );

    preview.Draw(canvas, {15, 40});
    debuff.Draw(canvas, {20, 45});
    debuffSurface.Draw(canvas);
    Font::DrawStringInBox(
        canvas,
        maps[userOpt.map].Debuff,
        {{20, 175}, {previewWidth - 10, 100}},
        1,
        0,
        (Color)14
    );

    mapTitleButton.Draw(canvas);
    if (userOpt.map != 0) {
        mapL.Draw(canvas);
    }
    if (userOpt.map !=  R.Config.MapUnlocked - 1) {
        mapR.Draw(canvas);
    }

    switch (currentMenu) {
        case 0:
            menu.Draw(canvas);
            break;
        case 1:
            diffMenu.Draw(canvas);
            break;
        case 2:
            musicMenu.Draw(canvas);
            break;
        case 3:
            modeMenu.Draw(canvas);
            break;
    }
}

void MapSelect::Unmount()
{
    audio.SwitchMusic(BGMusic::Menu);
    audio.PlayMusic();
}
