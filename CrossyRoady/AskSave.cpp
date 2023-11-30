#include "AskSave.h"

#include "GameMaster.h"
#include "GameType.h"
#include "GameUtils.h"
#include "SaveLoadType.h"

using namespace ConsoleGame;
using namespace GameType;
using namespace GameMaster;
using namespace GameUtils;

void AskSave::DataPeak()
{
    std::ifstream infile(SAVE_PATH, std::ios::binary);
    infile.read(reinterpret_cast<char*>(&currentTime), sizeof(time_t));
    infile.read(
        reinterpret_cast<char*>(&saveData.gameData), sizeof(GameMapData)
    );
    infile.read(reinterpret_cast<char*>(&saveData.character), sizeof(ValChar));
    infile.read(reinterpret_cast<char*>(&saveData.mapItem), sizeof(ValItem));
    infile.read(
        reinterpret_cast<char*>(&saveData.gameEventArgs), sizeof(GameEventsArgs)
    );

    // Reset the read pointer to the start of the file
    infile.seekg(0, std::ios::beg);
    infile.close();
}

void AskSave::DrawStat(AbstractCanvas* canvas) const
{
    Vec2 tmp = {112, 45};
    for (int i = 0; i < data.size(); i++) {
        Font::DrawString(canvas, data[i], tmp, 1, 0, Color(13));
        tmp.y += Font::GetDim(0).height + 3;
    }
}

const std::wstring_view AskSave::ScreenName() { return L"AskSave"; }

std::wstring_view AskSave::getName() { return ScreenName(); }

void AskSave::Init(const std::any& args)
{
    if (args.has_value()) {
        bg = std::any_cast<MenuBG*>(args);
    }
    DataPeak();
    menu.Init(
        {(384 - 100) / 2, 130},
        {100, 18},
        {R.String.Yes, R.String.No, R.String.Back}
    );
    Vec2 dim = Font::GetDim(0);
    auto strWidth = dim.width * R.String.AskSave.Continue.length();
    drawPos = {(384 - (int)strWidth) / 2, 20};
    surface = Surface(
        {.size = {(int)strWidth + 5, dim.height + 16},
         .pos = {drawPos.x - 5, drawPos.y - 8},
         .cornerSize = 7,
         .hasBorder = true,
         .background = (Color)14,
         .border = ((Color)13)}
    );

    surfaceData.props = {
        .size = {180, 80},
        .pos = {102, 40},
        .cornerSize = 5,
        .hasBorder = true,
        .background = Color(14),
        .border = Color(13)};

    constexpr size_t strLen = 27;
    std::string_view left[] = {
        R.String.AskSave.Date,
        R.String.AskSave.PlayTime,
        R.String.AskSave.Score,
        R.String.AskSave.Character,
        R.String.AskSave.Map,
        R.String.AskSave.Difficulty};

    char formattedTime[9];
    std::tm localTime;
    localtime_s(&localTime, &currentTime);
    std::strftime(formattedTime, sizeof(formattedTime), "%d/%m/%y", &localTime);
    std::string tmp[] = {"Auto", "Easy", "Normal", "Hard"};
    std::string tmp1[] = {"Fauna", "Irys", "Mumei", "Kronii", "Sana", "Bae"};
    std::string tmp2[] = {
        "Forest", "City", "House", "Desert", "Space", "Casino"};
    std::string right[] = {
        formattedTime,
        SecondsToMMSS(saveData.gameEventArgs.playTime),
        std::to_string(saveData.gameEventArgs.currentScore),
        tmp1[saveData.gameData.charaType],
        tmp2[saveData.gameData.mapType],
        tmp[saveData.gameData.mapDifficulty]};

    std::string spacePad = "";
    for (int i = 0; i < data.size(); i++) {
        spacePad.resize(strLen - left[i].size() - right[i].size() - 1, ' ');
        data[i] = std::format("{}:{}{}", left[i], spacePad, right[i]);
    }
}

AbstractScreen* AskSave::Clone() const { return new AskSave; }

AbstractNavigation::NavigationRes AskSave::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    if (bg) {
        bg->Update(deltaTime);
    }
    auto nav = navigation->NoChange();
    menu.Update(
        deltaTime,
        [&](uint8_t hovering) noexcept { audio.PlayHoverSfx(); },
        [&](uint8_t selected) noexcept {
            audio.PlayClickSfx();
            switch (selected) {
                case 0:
                    nav = navigation->Navigate(GameMap::ScreenName(), true);
                    break;
                case 1:
                    nav =
                        navigation->Navigate(CharacterSelectScreen::ScreenName()
                        );
                    break;
                case 2:
                    nav = navigation->Back();
                    break;
            }
        }
    );
    return nav;
}

void AskSave::Draw(AbstractCanvas* canvas) const
{
    if (bg) {
        bg->Draw(canvas);
    }
    surface.Draw(canvas);
    surfaceData.Draw(canvas);
    DrawStat(canvas);
    Font::DrawString(
        canvas, R.String.AskSave.Continue, drawPos, 1, 0, Color(13)
    );
    menu.Draw(canvas);
}
