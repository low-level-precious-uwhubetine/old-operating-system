#ifndef CRYSTALOS__APPLICATION__EXPLORER
#define CRYSTALOS__APPLICATION__EXPLORER

#include <graphics/desktop.h>
#include <controls/msgbox.h>
#include <controls/listbox.h>
#include <controls/label.h>
#include <util/events.h>

namespace crystalos
{
    namespace application
    {
        struct DriveEntry
        {
            char* Name;
            int Index;
            drivers::FileSystem* fs;
            char** fileArray;
            controls::ListBox* lb;
        }__attribute__((packed));

        class Explorer : public graphics::Window
        {
            private:
                controls::ListBox* driveList;
                DriveEntry* driveEntries;
            public:
                void OnKeyDown(char c);
                void OnKeyUp(char c);
                void Draw();

                void Init();

                void AddChild(graphics::Widget* child);
                static void driveList_SelectedIndexChanged(util::EventArgs*);
                static void fileList_SelectedIndexChanged(util::EventArgs*);

                Explorer(common::uint32_t* parent, common::uint32_t LocationX, common::uint32_t LocationY);
                ~Explorer();
        };
    }
}


#endif