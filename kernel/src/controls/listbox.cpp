#include <controls/listbox.h>

using namespace crystalos;
using namespace crystalos::graphics;
using namespace crystalos::controls;
using namespace crystalos::util;
using namespace crystalos::common;

ListBox::ListBox(Desktop* desktop, int locX, int locY, char* arr[])
: Widget((uint32_t*)desktop, ListBoxWidget, locX, locY)
{
    numItems = 0;
    for (numItems; numItems < 256; numItems++)
    {
        if (arr[numItems] == 0) break;
        Items[numItems] = arr[numItems];
    }
    Width = 84;
    Height = 104;

    parent = desktop;
    displayIndex = 0;
    if (numItems == 0) currentIndex = 0x100;
    else currentIndex = 0;

}

ListBox::ListBox(Desktop* desktop, int locX, int locY, int width, int height, char* arr[])
: Widget((uint32_t*)desktop, ListBoxWidget, locX, locY)
{
    numItems = 0;
    for (numItems; numItems < 256; numItems++)
    {
        if (arr[numItems] == 0) break;
        Items[numItems] = arr[numItems];
    }
    if (width > 84) Width = ((width/8) * 8) + 4;
    else Width = 84;
    if (height > 104) Height = ((height/20) * 20) + 4;
    else Height = 104;

    parent = desktop;
    displayIndex = 0;
    if (numItems == 0) currentIndex = 0x100;
    else currentIndex = 0;
}

ListBox::~ListBox()
{

}

void ListBox::Draw()
{
    parent->FillRectangle(XPos, YPos, Width, Height, 255, 255, 255);
    int maxItems = (Height - 4) / 20;
    int maxTextLength = (Width - 4) / 8;
    int xpos = 2, ypos = 4;
    for (int y = displayIndex; y < displayIndex + maxItems; y++)
    {
        if (Items[y] == 0) break;
        if (y == currentIndex)
            parent->FillRectangle(XPos + 2, YPos + 2 + ((currentIndex - displayIndex) * 20), Width - 4, 20, 0x90, 0x90, 0xFF);

        for (int x = 0; x < maxTextLength; x++)
        {
            if (Items[y][x] == 0) break;
            parent->DrawTransparentChar16(Items[y][x], XPos+xpos, YPos+ypos, 0, 0, 0);
            xpos+=8;
        }
        xpos = 2;
        ypos += 20;
    }
    if (Selected) drawBorder();
}

void ListBox::OnKeyDown(char c)
{
    int maxItems = (Height - 4) / 20;
    switch (c)
    {
        case '\n': //Enter
        {
            
            break;
        }
        case 0xf7: //up
        {
            int tmp = currentIndex;
            if (currentIndex > 0) currentIndex--;
            if (currentIndex < displayIndex)
            {
                displayIndex = currentIndex;
            }
            if (tmp != currentIndex) {
                if (isEventAssigned(util::SelectedIndexChanged))
                {
                    SelectedIndexChangedEventArgs* args = (SelectedIndexChangedEventArgs*)MemoryManager::ActiveMemoryManager->malloc(sizeof(SelectedIndexChangedEventArgs));
                    if (args == 0) return;
                    new (args) EventArgs();
                    void (*handler)(EventArgs*) = (void(*)(EventArgs*))events[util::SelectedIndexChanged];
                    
                    args->sender = (uint32_t*)this;
                    args->oldIndex = currentIndex + 1;
                    args->newIndex = currentIndex;
                    (*handler)(args);
                    MemoryManager::ActiveMemoryManager->free(args);
                } 
            }
            Draw();
            break;
        }
        case 0xf6: //down
        {
            int tmp = currentIndex;
            if (currentIndex < (numItems - 1)) currentIndex++;
            if (currentIndex >= displayIndex + maxItems)
            {
                displayIndex ++;
            } 
            if (tmp != currentIndex) 
            {
                if (isEventAssigned(util::SelectedIndexChanged))
                {
                    SelectedIndexChangedEventArgs* args = (SelectedIndexChangedEventArgs*)MemoryManager::ActiveMemoryManager->malloc(sizeof(SelectedIndexChangedEventArgs));
                    if (args == 0) return;
                    new (args) EventArgs();
                    void (*handler)(EventArgs*) = (void(*)(EventArgs*))events[util::SelectedIndexChanged];
                    
                    args->sender = (uint32_t*)this;
                    args->oldIndex = currentIndex - 1;
                    args->newIndex = currentIndex;
                    (*handler)(args);

                    MemoryManager::ActiveMemoryManager->free(args);
                }
            }
            Draw();
            break;
        }
    }
    
}

SelectedIndexChangedEventArgs::SelectedIndexChangedEventArgs()
{
    oldIndex = -1;
    newIndex = -1;
}
SelectedIndexChangedEventArgs::~SelectedIndexChangedEventArgs()
{}