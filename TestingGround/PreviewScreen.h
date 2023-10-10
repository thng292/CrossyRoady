#pragma once
#include "ConsoleGame.h"
#include <string_view>

class PreviewScreen : public ConsoleGame::AbstractScreen
{

    ConsoleGame::Sprite preview;
    const std::string_view fileName = "../spritegen/out.sprite";
    float timePassed = 2;
    std::filesystem::file_time_type lastWriteTime;
    ConsoleGame::Vec2 drawCoord;

public:
    static const std::wstring_view ScreenName();
    // Inherited via AbstractScreen
    std::wstring_view getName() override;
    void Init(const std::any& args) override;
    AbstractScreen* Clone() const override;
    ConsoleGame::AbstractNavigation::NavigationRes Update(float deltaTime, const ConsoleGame::AbstractNavigation* navigation) override;
    void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
    void DeInit() override;
};

