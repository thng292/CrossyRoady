#include "Navigation.h"

namespace ConsoleGame {
    AbstractNavigation::NavigationRes Navigation::NoChange() const
    {
        return NavigationRes{
            .ActionType = NavigationAction::None,
            .ActionData = L"",
            .Payload = std::any()};
    }

    AbstractNavigation::NavigationRes Navigation::Back(std::any payload) const
    {
        return NavigationRes{
            .ActionType = NavigationAction::Back,
            .ActionData = L"",
            .Payload = payload};
    }

    AbstractNavigation::NavigationRes Navigation::Navigate(
        std::wstring_view screenName, std::any payload
    ) const
    {
        return NavigationRes{
            .ActionType = NavigationAction::Navigate,
            .ActionData = screenName,
            .Payload = payload};
    }

    AbstractNavigation::NavigationRes Navigation::NavigatePopup(
        std::wstring_view screenName, std::any payload
    ) const
    {
        return NavigationRes{
            .ActionType = NavigationAction::NavigatePopup,
            .ActionData = screenName,
            .Payload = payload};
    }

    AbstractNavigation::NavigationRes Navigation::PopBackTo(
        std::wstring_view screenName, std::any payload
    ) const
    {
        return NavigationRes{
            .ActionType = NavigationAction::PopBackTo,
            .ActionData = screenName,
            .Payload = payload};
    }

    AbstractNavigation::NavigationRes Navigation::Exit() const
    {
        return NavigationRes{
            .ActionType = NavigationAction::Exit,
        };
    }
}  // namespace ConsoleGame
