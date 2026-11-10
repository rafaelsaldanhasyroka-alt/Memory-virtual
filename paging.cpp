#include "paging.h"


int GetPageNumber(int virtualAddress)
{
    return virtualAddress / PageSize;
}

int GetPageOffset(int address)
{
    return address % PageSize;
}

int GetFrameNumber(int physicalAddress)
{
    return physicalAddress / PageSize;
}

int GetPhysicalAddress(int frameNumber, int offset)
{
    return (frameNumber * PageSize) + offset;
}

int GetVirtualPage(int frameNumber)
{
    for (int i = 0; i < 8; i++)
    {
        if (PageTable[i] == frameNumber)
        {
            return i;
        }
    }
    return -1;
}

int GetVirtualAddress(int pageNumber, int offset)
{
    return (pageNumber * PageSize) + offset;
}


int FindFreeFrame()
{
    for (int i = 0; i < 4; i++)
    {
        int aux = GetVirtualPage(i); 
        if (aux < 0)
        {
            return i;
        }
    }
    return -1;
}

void SavePageToDisk(int pageNumber, int frameNumber)
{
    int diskStart = pageNumber * PageSize;
    int ramStart = frameNumber * PageSize;
    for (int i = 0; i < PageSize; i++)
    {
        Disk[diskStart + i] = RAM[ramStart + i];
    }
}

void LoadPageFromDisk(int pageNumber, int frameNumber)
{
    int diskStart =  pageNumber * PageSize;
    int ramstart = frameNumber * PageSize;
    for (int i = 0; i < PageSize; i++)
    {
        RAM [ramstart + i] = Disk[diskStart+ i];
    }
}


void ReplacePage(int newPageNumber, int frameNumber)
{
    int currentPage = GetVirtualPage(frameNumber);
    SavePageToDisk(currentPage, frameNumber);
    PageTable[currentPage] = -1;
    PageTable[newPageNumber] = frameNumber;
    LoadPageFromDisk(newPageNumber, frameNumber);
}

void UpdateFIFO(int newPageNumber)
{
    if (FIFOCount < 4)
    {
        FIFOQueue[FIFOCount] = newPageNumber;
        FIFOCount++;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            FIFOQueue[i] = FIFOQueue[i + 1];
        }
        FIFOQueue[3] = newPageNumber;
    }
}


void ReplaceOldestPage(int newPageNumber)
{
    int oldestPage = FIFOQueue[0];
    int frameNumber = PageTable[oldestPage];
    ReplacePage(newPageNumber, frameNumber);
    UpdateFIFO(newPageNumber);
}


bool LoadPage(int pageNumber)
{
    int aux = FindFreeFrame();
    if (aux == -1)
    {
        return false;
    }
    else
    {
        PageTable[pageNumber] = aux;
        UpdateFIFO(pageNumber);
        LoadPageFromDisk(pageNumber, aux);
        return true; 
    }
}


void InitializeDisk()
{
    for (int i = 0; i < VirtualMemorySize; i++)
    {
        Disk[i] = i;
    }
}
