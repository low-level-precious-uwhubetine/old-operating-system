#ifndef CRYSTALOS__CONTROLS__GUIEVENTMONITOR_H
#define CRYSTALOS__CONTROLS__GUIEVENTMONITOR_H

#include <common/types.h>
#include <graphics/widget.h>
#include <graphics/desktop.h>

namespace crystalos
{
    namespace controls
    {
        class GUIEventMonitor : public graphics::Widget
        {
            private:
                graphics::Desktop* Parent;
                char* TextBuffer;
                int y;
                int maxTextLength;
                int maxTextRows;
                int yIndex;

            public:

                void Draw();
                void Log(char* text);

                static GUIEventMonitor* KernelEventMonitor;

                GUIEventMonitor(common::uint32_t* parent, graphics::WidgetType type, int xpos, int ypos, int width, int height);
                ~GUIEventMonitor();
        };
    }
}


#endif