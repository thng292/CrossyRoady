#pragma once
#include "Abstract/AbstractNavigation.h"

namespace ConsoleGame {
    class Navigation : public AbstractNavigation {
       public:
        NavigationRes NoChange() const override;
        NavigationRes Back(std::any payload) const override;
        NavigationRes Navigate(std::wstring_view screenName, std::any payload)
            const override;
        NavigationRes NavigatePopup(
            std::wstring_view screenName, std::any payload
        ) const override;
        NavigationRes PopBackTo(std::wstring_view screenName, std::any payload)
            const override;

        // Inherited via AbstractNavigation
        NavigationRes Exit() const override;
    };
}  // namespace ConsoleGame
