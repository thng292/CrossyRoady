#pragma once

#include <any>
#include <string_view>

#include "../Common.h"
#include "AbstractCanvas.h"
#include "AbstractNavigation.h"

namespace ConsoleGame {
    class AbstractScreen {
       public:
        virtual std::wstring_view getName() = 0;

        virtual void Init(const std::any& args) = 0;  // called multiple time
        virtual AbstractScreen* Clone() const = 0;
        virtual void Mount(const std::any& args){}; // Only get args from back or pop back to
        // maybe called more than the target fps in 1 sec, should be fast
        // enough
        virtual AbstractNavigation::NavigationRes Update(
            float deltaTime, const AbstractNavigation* navigation
        ) = 0;
        // should be fast enough
        virtual void Draw(AbstractCanvas* canvas) const = 0;
        // cleaning up the temporaries, called multiple time
        virtual void Unmount(){};
    };
}  // namespace ConsoleGame
