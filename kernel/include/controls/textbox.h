#ifndef CRYSTALOS__CONTROLS__TEXTBOX_H
#define CRYSTALOS__CONTROLS__TEXTBOX_H

#include <graphics/desktop.h>

/*
    The TextBox Control
        Insert text into an application using keys on the keyboard
*/

namespace crystalos
{
    namespace controls
    {
        class TextBox : public graphics::Widget
        {
            protected:
                graphics::Desktop* parent;
                int TextLength;
                
                common::uint32_t currentIndex;
                common::uint32_t displayStart; 
                common::uint32_t cursorPos; 
                
                char* Text;
                int maxChars;   

            public:
                void Draw();
                void OnKeyDown(char c);
                void GetText(char*);
                void SetText(char*);
                void onFocus();
                TextBox(graphics::Desktop*, int locX, int locY);
                TextBox(graphics::Desktop*, int locX, int locY, int width);
                ~TextBox();
        };
    }
}


#endif