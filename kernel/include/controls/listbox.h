#ifndef CRYSTALOS__CONTROLS__LISTBOX_H
#define CRYSTALOS__CONTROLS__LISTBOX_H

#include <graphics/desktop.h>
#include <util/system.h>
#include <util/events.h>

/*
    The ListBox Control
        Lists items specified and be able to scroll through them using the keyboard.
        Carry out events when keys are pressed or a mouse button is clicked.
*/

namespace crystalos
{
    namespace controls
    {
        class SelectedIndexChangedEventArgs: public util::EventArgs
        {
            public:
                int oldIndex;
                int newIndex;
                SelectedIndexChangedEventArgs();
                ~SelectedIndexChangedEventArgs();
        };

        class ListBox : public graphics::Widget
        {
            protected:
                graphics::Desktop* parent;                
                common::uint32_t displayIndex;
                int numItems;

            public:
                common::uint32_t currentIndex;
                void Draw();
                void OnKeyDown(char c);

                char* Items[256];
                ListBox(graphics::Desktop*, int locX, int locY, char* arr[]);
                ListBox(graphics::Desktop*, int locX, int locY, int , int , char* arr[]);
                ~ListBox();
        };
    }
}


#endif