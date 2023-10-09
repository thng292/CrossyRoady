#pragma once

#include <any>
#include <string_view>

namespace ConsoleGame {
    class AbstractNavigation {
       public:
        enum class NavigationAction : char {
            Back,
            Navigate,
            NavigatePopup,
            PopBackTo,
            None,
        };

        struct NavigationRes {
            NavigationAction ActionType;
            std::wstring_view ActionData;
            std::any Payload;
        };

        virtual NavigationRes NoChange() const = 0;

        virtual NavigationRes Back(std::any payload = std::any()) const = 0;

        virtual NavigationRes Navigate(
            std::wstring_view screenName, std::any payload = std::any()
        ) const = 0;

        virtual NavigationRes NavigatePopup(
            std::wstring_view screenName, std::any payload = std::any()
        ) const = 0;

        virtual NavigationRes PopBackTo(
            std::wstring_view screenName, std::any payload = std::any()
        ) const = 0;
    };
}  // namespace ConsoleGame
