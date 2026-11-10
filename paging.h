#ifndef PAGING_H
#define PAGING_H

namespace paging {
int GetPageNumber(int virtualAddress);
int GetPageOffset(int address);
int GetFrameNumber(int physicalAddress);
int GetPhysicalAddress(int frameNumber, int offset);
int GetVirtualPage(int frameNumber);
int GetVirtualAddress(int pageNumber, int offset);
int FindFreeFrame();
void SavePageToDisk(int pageNumber, int frameNumber);
void LoadPageFromDisk(int pageNumber, int frameNumber);
void ReplacePage(int newPageNumber, int frameNumber);
void UpdateFIFO(int newPageNumber);
void ReplaceOldestPage(int newPageNumber);
bool LoadPage(int pageNumber);
void InitializeDisk();
};

extern char Disk[2048] = {};
extern char RAM[1024] = {};
extern int PageFaultCount = 0;
extern int VirtualMemorySize = 2048;
extern int PhysicalMemorySize = 1024;
extern int PageSize = 256;
extern int PageTable[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
extern int FIFOQueue[4] = {-1, -1, -1, -1};
extern int FIFOCount = 0;



#endif