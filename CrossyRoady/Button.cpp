#include "Button.h"

using namespace ConsoleGame;

Button::Button(
    SurfaceArgs surfaceArgs,
    const std::string_view& text,
    Color textColor,
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
    auto FontDim = Font::GetDim(fontVariant);
    FontDim.width *= fontSize;
    FontDim.height *= fontSize;
    int fWidth = FontDim.width * text.length();
    textPos.x = (props.size.width - fWidth) / 2 + props.pos.x;
    textPos.y = (props.size.height - FontDim.height) / 2 + props.pos.y;
}

void Button::ChangeTextColor(Color textColor) { this->textColor = textColor; }

bool Button::IsHover(Vec2 mousePos) const { return Surface::IsHover(mousePos); }

void Button::ChangeSurface(const SurfaceArgs& surface)
{
    Surface::props = surface;
    calculateTextPos();
}

void Button::ChangeColor(
    Color textColor, Color backgroundColor, Color borderColor
)
{
    this->textColor = textColor;
    props.background = backgroundColor;
    props.border = borderColor;
}

void Button::ChangeText(const std::string_view& text)
{
    this->text = text;
    calculateTextPos();
}

void Button::Draw(ConsoleGame::AbstractCanvas* canvas) const
{
    Surface::Draw(canvas);
    Font::DrawString(canvas, text, textPos, fontSize, fontVariant, textColor);
}
