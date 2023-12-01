#ifndef CRYSTALOS__CONTROLS__LABEL_H
#define CRYSTALOS__CONTROLS__LABEL_H

/*
    The label control.
    Used to display a block of text with the specified background and foreground color
        at the specified location in the current graphics mode on the screen.
*/

#include <graphics/desktop.h>

namespace crystalos
{
    namespace controls
    {
        class Label : public graphics::Widget
        {
            protected:
                void OnKeyDown(char);
                void OnKeyUp(char);
            
            public:
                char Text[256];
                Label(common::uint32_t* parent, int, int);
                ~Label();

                void onFocus();
                void SetText(char* text);
                void SetBackground(common::uint8_t, common::uint8_t, common::uint8_t);
                void SetForeground(common::uint8_t, common::uint8_t, common::uint8_t);
                void Draw();
        };
    }
}

#endif