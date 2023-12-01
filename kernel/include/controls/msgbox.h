#ifndef CRYSTALOS__CONTROLS__MSGBOX
#define CRYSTALOS__CONTROLS__MSGBOX

#include <graphics/desktop.h>
#include <util/events.h>

namespace crystalos
{
    namespace controls
    {
        class MsgBox : public graphics::Window
        {
            private:
                int maxTextLength;
                int maxTextRows;
                char* TextBuffer;
                int y, yIndex;
                common::uint32_t* db;
            public:
                void OnKeyDown(char c);
                
                void Draw(char* txt);
                static MsgBox* Alert;
                bool isShowing;

                MsgBox(common::uint32_t* parent, common::uint32_t LocationX, common::uint32_t LocationY);
                ~MsgBox();
        };
    }
}


#endif