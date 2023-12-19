#include "Button.h"

Button::Button(
    SurfaceArgs surfaceArgs,
    const std::string_view text,
    char textColor,
    uint8_t fontVariant,
    uint8_t fontSize
)
    : Surface(surfaceArgs),
      text(text),
      textColor(textColor),
      fontVariant(fontVariant),
      fontSize(fontSize)
{
    calculateTextPos();
}

void Button::calculateTextPos()
{
    auto FontDim = ConsoleGame::Font::GetDim(fontVariant);
    FontDim.width *= fontSize;
    FontDim.height *= fontSize;
    int fWidth = FontDim.width * text.length();
    textPos.x = (props.size.width - fWidth) / 2 + props.pos.x;
    textPos.y = (props.size.height - FontDim.height) / 2 + props.pos.y;
}

void Button::ChangeTextColor(char textColor) { this->textColor = textColor; }

bool Button::IsHover(ConsoleGame::Vec2 mousePos) const
{
    return Surface::IsHover(mousePos);
}

void Button::ChangeColor(char textColor, char backgroundColor, char borderColor)
{
    this->textColor = textColor;
    props.background = backgroundColor;
    props.border = borderColor;
}

void Button::ChangeText(const std::string_view text)
{
    this->text = text;
    calculateTextPos();
}

void Button::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    Surface::Draw(canvas);
    ConsoleGame::Font::DrawString(
        canvas, text, textPos, fontSize, fontVariant, textColor
    );
}
