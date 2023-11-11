#include "ConsoleGame.h"
#include"Button.h"
class Control : public ConsoleGame::AbstractScreen {
    Button test;
   public:
    static const std::wstring_view ScreenName();
    virtual std::wstring_view getName() override;
    virtual void Init(const std::any& args) override;
    virtual AbstractScreen* Clone() const override;
    virtual ConsoleGame::AbstractNavigation::NavigationRes Update(
        float deltaTime, const ConsoleGame::AbstractNavigation* navigation
    ) override;
    virtual void Draw(ConsoleGame::AbstractCanvas* canvas) const override;
};