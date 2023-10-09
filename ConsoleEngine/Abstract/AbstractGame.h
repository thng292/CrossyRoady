#pragma once

#include <string_view>

#include "AbstractScreen.h"

namespace ConsoleGame {

    class AbstractGame {
       public:
        virtual void Init() = 0;
        virtual void Run(std::wstring_view screenName) = 0;

        virtual AbstractGame* addScreen(std::unique_ptr<AbstractScreen>) = 0;
    };

}  // namespace ConsoleGame