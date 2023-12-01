
#include <controls/label.h>

using namespace crystalos::common;
using namespace crystalos::graphics;
using namespace crystalos::controls;
using namespace crystalos::util;

Label::Label(uint32_t* parent, int xpos, int ypos)
: Widget(parent, LabelWidget, xpos, ypos)
{
    Height = 18;
}

Label::~Label()
{

}

void Label::OnKeyDown(char c)
{

}

void Label::OnKeyUp(char c)
{

}

void Label::SetText(char* text)
{
    for (int i = 0; i < 256 || text[i] != '\0'; i++)
    {
        Text[i] = text[i];
    }
}

void Label::SetBackground(uint8_t r, uint8_t g, uint8_t b)
{
    BackgroundR = r;
    BackgroundG = g;
    BackgroundB = b;
}

void Label::SetForeground(uint8_t r, uint8_t g, uint8_t b)
{
    ForegroundR = r;
    ForegroundG = g;
    ForegroundB = b;
}

void Label::Draw()
{
    Width = len(Text) * 8 + 3;
    ((Desktop*)Parent)->FillRectangle(XPos, YPos, Width, Height, BackgroundR, BackgroundG, BackgroundB);
    ((Desktop*)Parent)->DrawText(Text, XPos + 2, YPos + 1, ForegroundR, ForegroundG, ForegroundB);
}

void Label::onFocus()
{

}