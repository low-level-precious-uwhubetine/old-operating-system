
#include <application/explorer.h>
#include <controls/gui_event_monitor.h>

using namespace crystalos;
using namespace crystalos::common;
using namespace crystalos::graphics;
using namespace crystalos::application;
using namespace crystalos::controls;
using namespace crystalos::util;
using namespace crystalos::drivers;

Explorer* currentExplorer = 0;
char* MassStorageTxt = "USB Mass Storage";

Explorer::Explorer(uint32_t* parent, uint32_t locX, uint32_t locY)
: Window(parent, locX, locY)
{
   
}

Explorer::~Explorer()
{

}

void Explorer::Init()
{
    BackgroundR = 28;
    BackgroundG = 12;
    BackgroundB = 170;

    driveList = (ListBox*)MemoryManager::ActiveMemoryManager->malloc(sizeof(ListBox));
    driveEntries = (DriveEntry*)MemoryManager::ActiveMemoryManager->malloc(sizeof(DriveEntry) * 31);
    MemoryManager::ActiveMemoryManager->memset((uint8_t*)driveEntries, 0, sizeof(DriveEntry) * 31);

    char** arr = (char**)MemoryManager::ActiveMemoryManager->malloc(sizeof(char**) * 31);
    
    int index = 0;

    GUIEventMonitor::KernelEventMonitor->Log("Detected disks:");
    if (IDEDriver::KernelIDEDriver != 0)
    {
        IDEDriver* ide_driver = IDEDriver::KernelIDEDriver; 
        if (!(ide_driver->primaryChannel.masterDrive.IsEmpty()))
		{
            GUIEventMonitor::KernelEventMonitor->Log(ide_driver->primaryChannel.masterDrive.ManufacturerModel);
            
            driveEntries[index].Name = ide_driver->primaryChannel.masterDrive.ManufacturerModel;
            driveEntries[index].Index = 0;
            index++;
		}
        if (!(ide_driver->primaryChannel.slaveDrive.IsEmpty()))
        {
            GUIEventMonitor::KernelEventMonitor->Log(ide_driver->primaryChannel.slaveDrive.ManufacturerModel);
            driveEntries[index].Name = ide_driver->primaryChannel.slaveDrive.ManufacturerModel;
            driveEntries[index].Index = 1;
            index++;
        }
        if (!(ide_driver->secondaryChannel.masterDrive.IsEmpty()))
        {
            GUIEventMonitor::KernelEventMonitor->Log(ide_driver->secondaryChannel.masterDrive.ManufacturerModel);
            driveEntries[index].Name = ide_driver->secondaryChannel.masterDrive.ManufacturerModel;
            driveEntries[index].Index = 2;
            index++;
        }
        if (!(ide_driver->secondaryChannel.slaveDrive.IsEmpty()))
        {
            GUIEventMonitor::KernelEventMonitor->Log(ide_driver->secondaryChannel.slaveDrive.ManufacturerModel);
            driveEntries[index].Name = ide_driver->secondaryChannel.slaveDrive.ManufacturerModel;
            driveEntries[index].Index = 3;
            index++;
        }
    }
    if (AHCIDriver::KernelAHCI != 0)
    {
        for (int j = 0; j < AHCIDriver::KernelAHCI->numDisks; j++)
        {
            GUIEventMonitor::KernelEventMonitor->Log(AHCIDriver::KernelAHCI->disks[j]->ManufacturerModel);
            driveEntries[index].Name = AHCIDriver::KernelAHCI->disks[j]->ManufacturerModel;
            driveEntries[index].Index = 0x21 + j;
            index++;
        }
    }
    
    if (EHCIDriver::OSEHCIDriver != 0)
    {
        for (int j = 0; j < 127; j++)
        {
            if (USBMassStorageDevices::Devices->MassStorageDevices[j] == 0) break;
            GUIEventMonitor::KernelEventMonitor->Log("USB Mass Storage");
            driveEntries[index].Name = MassStorageTxt;
            driveEntries[index].Index = 0x81 + j;
            index++;
        }
    }

    for (int j = 0; j < 31; j++) arr[j] = driveEntries[j].Name;
    
    new (driveList) ListBox((Desktop*)Parent, LocationX + 5, LocationY + 5, SizeX - 12, SizeY / 3, arr);
    AddHandler(driveList, SelectedIndexChanged, &driveList_SelectedIndexChanged);
    AddChild(driveList);

    for (int j = 0; j < 31; j++)
    {
        if (driveEntries[j].Name == 0) break;

        if (driveEntries[j].Index > 0x80)   //USB mass storage devices
        {

        }
        else if (driveEntries[j].Index > 0x20)  //AHCI drives
        {
            uint8_t* buffer = (uint8_t*)MemoryManager::ActiveMemoryManager->malloc(513);
            OSBootSector* boot_sector = (OSBootSector*)buffer;

            AHCIDriver::KernelAHCI->disks[driveEntries[j].Index-0x21]->Read(0, 1, buffer);
            if (boot_sector->partition2.FSType == 0x07)
            {
                FileSystem* fs = (FileSystem*)MemoryManager::ActiveMemoryManager->malloc(sizeof(FileSystem));
                new (fs) FileSystem(AHCIDriver::KernelAHCI->disks[driveEntries[j].Index-0x21], boot_sector);
                int num_files;
                File* files = fs->GetFiles("C:\\", &num_files);
                
                char** arr1 = (char**)MemoryManager::ActiveMemoryManager->malloc(sizeof(char**) * (num_files + 1));
                for (int i = 0; i < num_files; i++)
                {
                    arr1[i] = files[i].Name;
                }
                arr1[num_files] = 0;
                driveEntries[j].fs = fs;
                driveEntries[j].fileArray = arr1;
                driveEntries[j].lb = (ListBox*)MemoryManager::ActiveMemoryManager->malloc(sizeof(ListBox));

                new (driveEntries[j].lb) ListBox((Desktop*)Parent, LocationX + 5, LocationY + 10 + driveList->Height, SizeX - 12, SizeY / 3, driveEntries[j].fileArray);
                AddHandler(driveEntries[j].lb, SelectedIndexChanged, &fileList_SelectedIndexChanged);

            }
            else
            {
                MemoryManager::ActiveMemoryManager->free(buffer);
                driveEntries[j].fs = 0;
                driveEntries[j].fileArray = 0;
            }
        }
        else    //IDE drives
        {
            IDEDriver* ide_driver = IDEDriver::KernelIDEDriver;
            IDEDevice* disk = 0;
            switch(driveEntries[j].Index)
            {
                case 0:
                    disk = &ide_driver->primaryChannel.masterDrive;
                    break;
                case 1:
                    disk = &ide_driver->primaryChannel.slaveDrive;
                    break;
                case 2:
                    disk = &ide_driver->secondaryChannel.masterDrive;
                    break;
                case 3:
                    disk = &ide_driver->secondaryChannel.slaveDrive;
                    break;
            }

            uint8_t* buffer = (uint8_t*)MemoryManager::ActiveMemoryManager->malloc(513);
            OSBootSector* boot_sector = (OSBootSector*)buffer;

            disk->Read(0, 1, buffer);
            if (boot_sector->partition2.FSType == 0x07)
            {
                FileSystem* fs = (FileSystem*)MemoryManager::ActiveMemoryManager->malloc(sizeof(FileSystem));
                new (fs) FileSystem(disk, boot_sector);
                int num_files;
                File* files = fs->GetFiles("C:\\", &num_files);
                
                char** arr1 = (char**)MemoryManager::ActiveMemoryManager->malloc(sizeof(char**) * (num_files + 1));
                for (int i = 0; i < num_files; i++)
                {
                    arr1[i] = files[i].Name;
                }
                arr1[num_files] = 0;
                driveEntries[j].fs = fs;
                driveEntries[j].fileArray = arr1;
                driveEntries[j].lb = (ListBox*)MemoryManager::ActiveMemoryManager->malloc(sizeof(ListBox));

                new (driveEntries[j].lb) ListBox((Desktop*)Parent, LocationX + 5, LocationY + 10 + driveList->Height, SizeX - 12, SizeY / 3, driveEntries[j].fileArray);
                AddHandler(driveEntries[j].lb, SelectedIndexChanged, &fileList_SelectedIndexChanged);

            }
            else
            {
                MemoryManager::ActiveMemoryManager->free(buffer);
                driveEntries[j].fs = 0;
                driveEntries[j].fileArray = 0;
            }
        }
    }

    if (driveEntries[0].fs != 0)
    {
        AddChild(driveEntries[0].lb);
    }
    
    currentExplorer = this;
}

void Explorer::AddChild(Widget* child)
{
    Children[lastChildIndex] = child;
    lastChildIndex++;
}

void Explorer::OnKeyDown(char c)
{
    if (c == '\t')
    {
        if (lastChildIndex == 0) return;
        if (currentChild == lastChildIndex - 1) currentChild = 0;
        else currentChild++;
        if (currentChild > -1) Children[currentChild]->drawBorder();
        if (currentChild >= 1)
        {
            Children[currentChild-1]->clearBorder();
        }
    }
    else
    {
        if (currentChild > -1) 
        {
            Children[currentChild]->OnKeyDown(c);
            Children[currentChild]->Selected = true;
        }
    }
}

void Explorer::OnKeyUp(char c)
{

}

void Explorer::Draw()
{
    ((Desktop*)Parent)->FillRectangle(LocationX, LocationY, SizeX, SizeY, BackgroundR, BackgroundG, BackgroundB);

    for (int i = 0; i < lastChildIndex; i++)
    {
        Children[i]->Draw();
    }
    if (currentChild > -1) Children[currentChild]->drawBorder();

    if (Selected) drawBorder();
}

void Explorer::driveList_SelectedIndexChanged(EventArgs* args)
{
    SelectedIndexChangedEventArgs* argv = (SelectedIndexChangedEventArgs*)args;
    if (currentExplorer->lastChildIndex == 2) 
    {
        currentExplorer->Children[currentExplorer->lastChildIndex] = 0;
        currentExplorer->lastChildIndex--;
    }

    currentExplorer->AddChild(currentExplorer->driveEntries[argv->newIndex].lb);
    currentExplorer->Draw();
}

void Explorer::fileList_SelectedIndexChanged(EventArgs* args)
{
    
    SelectedIndexChangedEventArgs* argv = (SelectedIndexChangedEventArgs*)args;

    MsgBox::Alert->Draw("This is a file");
}