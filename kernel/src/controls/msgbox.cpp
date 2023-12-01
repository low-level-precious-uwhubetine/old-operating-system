#include <controls/msgbox.h>
#include <controls/gui_event_monitor.h>

using namespace crystalos;
using namespace crystalos::common;
using namespace crystalos::controls;
using namespace crystalos::graphics;
using namespace crystalos::util;

MsgBox* MsgBox::Alert = 0;

MsgBox::MsgBox(uint32_t* parent, uint32_t LocX, uint32_t LocY)
: Window(parent, LocX, LocY)
{
    BackgroundR = 134;
    BackgroundG = 173;
    BackgroundB = 4;
    SizeX = 500;
    SizeY = 500;

    maxTextLength = (SizeX - 8) / 8;
    maxTextRows = (SizeY - 8) / 20;
    TextBuffer = (char*)MemoryManager::ActiveMemoryManager->malloc(maxTextLength*maxTextRows + maxTextLength);
    MemoryManager::ActiveMemoryManager->memset((uint8_t*)TextBuffer, 0, maxTextLength*maxTextRows + maxTextLength);

    y = 4;
    yIndex = 0;
    isShowing = false;

    if (Alert == 0) Alert = this;
    db = (uint32_t*)MemoryManager::ActiveMemoryManager->malloc(SizeX*SizeY*5);
}

MsgBox::~MsgBox()
{
    if (Alert == this) Alert = 0;
}

void MsgBox::OnKeyDown(char c)
{
    if ((uint8_t)c == 0xf3)  //esc
    {
        int in = 0;
        for (int x = LocationX; x < LocationX+SizeX; x++)
            for (int y = LocationY; y < LocationY+SizeY; y++)
            {
                ((Desktop*)Parent)->screenMemory[y* ((Desktop*)Parent)->currentMode->XResolution + x] = db[in];
                in++;
            }
        isShowing = false;
    }
}

void MsgBox::Draw(char* text)
{
    int in = 0;
    for (int x = LocationX; x < LocationX+SizeX; x++)
        for (int y = LocationY; y < LocationY+SizeY; y++)
        {
            db[in] = ((Desktop*)Parent)->doubleBuffer[y* ((Desktop*)Parent)->currentMode->XResolution + x];
            in++;
        }
    isShowing = true;
    ((Desktop*)Parent)->FillRectangle(LocationX, LocationY, SizeX, SizeY, BackgroundR, BackgroundG, BackgroundB);
    int textLength = len(text);
    if (textLength == 0) return;
    char* buf = (char*)MemoryManager::ActiveMemoryManager->malloc(textLength+1);
    char* buf1 = (char*)MemoryManager::ActiveMemoryManager->malloc(maxTextLength+1);
    int numRows = textLength / maxTextLength;
    if (textLength % maxTextLength != 0) numRows++;

    for (int i = 0; i < numRows; i++)
    {
        MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)text, (uint8_t*)buf, textLength+1);
        buf = subString(buf,i * maxTextLength, maxTextLength);
        MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)buf, (uint8_t*)buf1, maxTextLength);
        buf1[maxTextLength] = 0;
        ((Desktop*)Parent)->DrawText(buf1, LocationX+4, LocationY + y, 0, 0, 0);
        MemoryManager::ActiveMemoryManager->memcpy((uint8_t*)buf1, (uint8_t*)(TextBuffer+yIndex*maxTextLength), maxTextLength);
        y += 20;
        yIndex++;
    }
    MemoryManager::ActiveMemoryManager->free(buf);
    MemoryManager::ActiveMemoryManager->free(buf1);
}
