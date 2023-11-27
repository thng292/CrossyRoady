#include "AskSave.h"

using namespace ConsoleGame;

const std::wstring_view AskSave::ScreenName() { return L"AskSave"; }

std::wstring_view AskSave::getName() { return ScreenName(); }

void AskSave::Init(const std::any& args)
{
    if (args.has_value()) {
        bg = std::any_cast<MenuBG*>(args);
    }
    menu.Init(
        {(384 - 100) / 2, 80},
        {100, 18},
        {R.String.Yes, R.String.No, R.String.Back}
    );
    Vec2 dim = Font::GetDim(0);
    drawPos = {
        (384 - (int)(dim.width * R.String.AskSave.Continue.length())) / 2, 50};
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
    Font::DrawString(
        canvas, R.String.AskSave.Continue, drawPos, 1, 0, Color(14)
    );
    menu.Draw(canvas);
}
