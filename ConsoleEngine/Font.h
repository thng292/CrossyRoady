//#pragma once
//#include <filesystem>
//#include <vector>
//
//#include "Common.h"
//#include "Sprite.h"
//
//namespace ConsoleGame {
//    class Font {
//       public:
//        // static constexpr Vec2 fontDim[2] = {
//        //     {.width = 6,  .height = 9 },
//        //     {.width = 12, .height = 18},
//        // };
//        static constexpr char CharacterCount = 26;
//        static constexpr char SymbolCount = 18;
//        static constexpr char DigitCount = 10;
//
//        static constexpr uint8_t lineHeight[2] = {14, 26};
//        static constexpr uint8_t FontWidth[2][26] = {0};
//        enum class Variant {
//            Small = 0,
//            Large = 1,
//        };
//
//       private:
//        static std::vector<bool> data[2];
//
//       public:
//        static uint8_t GetFontHeight(char ch);
//
//        static void Load(std::filesystem::path path, Variant variant);
//        static Sprite GenerateSprite(
//            std::string_view str, Variant variant = Variant::Small
//        );
//    };
//
//}  // namespace ConsoleGame
