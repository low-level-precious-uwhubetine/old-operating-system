#include <controls/textbox.h>

using namespace crystalos;
using namespace crystalos::common;
using namespace crystalos::controls;
using namespace crystalos::util;
using namespace crystalos::graphics;

TextBox::TextBox(Desktop* desktop, int locX, int locY)
: Widget((uint32_t*)desktop, TextBoxWidget, locX, locY)
{
    Text = (char*)MemoryManager::ActiveMemoryManager->malloc(257);
    for (int i = 0; i < 256; i++) Text[i] = 0;
    if (len(Text) > 0) currentIndex = len(Text) - 1;
    else currentIndex = 0;
    parent = desktop;
    
    cursorPos = 1;
    displayStart = 0;

    Width = 100;
    Height = 20;

    maxChars = (Width - 4) / 8;

    if (currentIndex >= maxChars) displayStart = currentIndex - maxChars;
    TextLength = len(Text);
}

TextBox::TextBox(Desktop* desktop, int locX, int locY, int width)
: Widget((uint32_t*)desktop, TextBoxWidget, locX, locY)
{
    Text = (char*)MemoryManager::ActiveMemoryManager->malloc(257);
    for (int i = 0; i < 256; i++) Text[i] = 0;
    if (len(Text) > 0) currentIndex = len(Text) - 1;
    else currentIndex = 0;
    parent = desktop;
    
    cursorPos = 1;
    displayStart = 0;

    Width = (((width - 4) / 8) * 8) + 4;
    Height = 20;

    maxChars = (Width - 4) / 8;

    if (currentIndex >= maxChars) displayStart = currentIndex - maxChars;
    TextLength = len(Text);

}

TextBox::~TextBox()
{

}

void TextBox::Draw()
{
    int tmp = 2;
    
    parent->FillRectangle(XPos, YPos, Width, Height, 0xFF, 0xFF, 0xFF);

    for (int i = displayStart; i < displayStart + maxChars; i++)
    {
        if (Text[i] == 0) break;
        parent->DrawTransparentChar16(Text[i], XPos + tmp, YPos + 2, 0, 0, 0);
        tmp += 8;
    }
    for (uint8_t y = YPos + 2; y < YPos + Height - 2; y++)
        parent->PutPixel(XPos + cursorPos, y, 0, 0, 0);

    if (Selected) drawBorder();    
}

void TextBox::OnKeyDown(char c)
{
    switch(c)
    {
        case '\b':
            if (currentIndex > 0)
            {
                TextLength--;
                for (int i = currentIndex; i < 256; i++)
                {
                    Text[i-1] = Text[i];
                }
                Text[TextLength] = 0;
                currentIndex--;
                if (displayStart > 0) displayStart--;
                else
                    if (cursorPos > 1) cursorPos -= 8;
                
                Draw();
            }

            break;
        case '\n':
            break;
        case 0xf6: case 0xf7: break;
        case 0xf8:  //Left
            if (cursorPos > 1) cursorPos -= 8;
            else
            {
                if (displayStart > 0) displayStart--;
            }
            if (currentIndex > 0) currentIndex--;
            Draw();
            break;
        case 0xf9:  //Right
            if (currentIndex < TextLength)
            {
                currentIndex++;
                if ((cursorPos < Width - 10)) cursorPos += 8;
                else
                {
                    if (TextLength - displayStart > maxChars) displayStart++;
                }
                Draw();
            }
            break;
        default:
            if (TextLength < 255)
            {
                Text = insertChar(c, Text, currentIndex);
                currentIndex++;
                TextLength++;
                Text[TextLength] = 0;
                if (cursorPos < Width - 4) cursorPos += 8;
                else displayStart++;
                Draw();
            }   
            break;
    }
}


void TextBox::GetText(char* out)
{
    MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)Text, (uint8_t*)out, 256);
}

void TextBox::SetText(char* in)
{
    MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)in, (uint8_t*)Text, 256);
}

void::TextBox::onFocus()
{
    
}