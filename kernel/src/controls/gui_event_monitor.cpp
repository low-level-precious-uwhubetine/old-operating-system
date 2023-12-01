
#include <controls/gui_event_monitor.h>

using namespace crystalos;
using namespace crystalos::controls;
using namespace crystalos::common;
using namespace crystalos::graphics;
using namespace crystalos::util;

GUIEventMonitor* GUIEventMonitor::KernelEventMonitor = 0;

GUIEventMonitor::GUIEventMonitor(uint32_t* parent, WidgetType type, int xpos, int ypos, int width, int height)
: Widget(parent, type, xpos, ypos)
{
    Parent = (Desktop*)parent;

    Width = width;
    Height = height;

    BackgroundR = 33;
    BackgroundG = 34;
    BackgroundB = 33;

    ForegroundR = 12;
    ForegroundG = 165;
    ForegroundB = 12;

    maxTextLength = (Width - 8) / 8;
    maxTextRows = (Height - 8) / 20;
    TextBuffer = (char*)MemoryManager::ActiveMemoryManager->malloc(maxTextLength*maxTextRows + maxTextLength);
    MemoryManager::ActiveMemoryManager->memset((uint8_t*)TextBuffer, 0, maxTextLength*maxTextRows + maxTextLength);

    y = 4;
    yIndex = 0;

    if (KernelEventMonitor == 0) KernelEventMonitor = this;
}

GUIEventMonitor::~GUIEventMonitor()
{

}

void GUIEventMonitor::Draw()
{
    char* text = (char*)MemoryManager::ActiveMemoryManager->malloc(maxTextLength+1);
    Parent->FillRectangle(XPos, YPos, Width, Height, BackgroundR, BackgroundG, BackgroundB);
    for (int i = 0; i < maxTextRows; i++)
    {
        MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)(TextBuffer+i*maxTextLength), (uint8_t*)text, maxTextLength);
        text[maxTextLength] = 0;
        Log(text);
    }
    MemoryManager::ActiveMemoryManager->free(text);
}

void GUIEventMonitor::Log(char* text)
{
    int textLength = len(text);
    if (textLength == 0) return;
    char* buf = (char*)MemoryManager::ActiveMemoryManager->malloc(textLength+1);
    char* buf1 = (char*)MemoryManager::ActiveMemoryManager->malloc(maxTextLength+1);
    int numRows = textLength / maxTextLength;
    if (textLength % maxTextLength != 0) numRows++;

    for (int i = 0; i < numRows; i++)
    {
        if (y >= Height - 4)
        {
            y = 4;
            yIndex -= maxTextRows;
            MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)(TextBuffer+maxTextLength), (uint8_t*)TextBuffer, maxTextLength*maxTextRows);
            Draw();
        }

        MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)text, (uint8_t*)buf, textLength+1);
        buf = subString(buf,i * maxTextLength, maxTextLength);
        MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)buf, (uint8_t*)buf1, maxTextLength);
        buf1[maxTextLength] = 0;
        Parent->DrawText(buf1, XPos+4, YPos + y, ForegroundR, ForegroundG, ForegroundB);
        MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)buf1, (uint8_t*)(TextBuffer+yIndex*maxTextLength), maxTextLength);
        y += 20;
        yIndex++;
    }
    MemoryManager::ActiveMemoryManager->free(buf);
    MemoryManager::ActiveMemoryManager->free(buf1);
}