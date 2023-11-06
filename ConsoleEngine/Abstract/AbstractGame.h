#pragma once

#include <string_view>

#include "AbstractScreen.h"

namespace ConsoleGame {

    class AbstractGame {
       public:
        virtual void init() = 0;
        virtual void Run(std::wstring_view screenName) = 0;

        virtual AbstractGame* AddScreen(std::unique_ptr<AbstractScreen>) = 0;
    };

}  // namespace ConsoleGame