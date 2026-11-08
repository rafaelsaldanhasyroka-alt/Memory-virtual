
#include <iostream>

int VirtualMemorySize = 2048;
int PhysicalMemorySize = 1024;
int PageSize = 256;
int PageTable[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
int FIFOQueue[4] = {-1, -1, -1, -1};
int FIFOCount = 0;
char Disk[2048] = {};
char RAM[1024] = {};
int PageFaultCount = 0;

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

int main()
{
    int option;
    bool menu = true;
    InitializeDisk();
    Disk[600] = 75;
    
    while (menu)
    {
        std::cout << " MENU FODASSE \n";
        std::cout << "1 - Converter endereco virtual para fisico \n";
        std::cout << "2 - Converter endereco fisico para virtual \n";
        std::cout << "3 - Visualizar dado armazenado na memoria \n";
        std::cout << "4 - Visualizar quantidade de Page faults \n";
        std::cout << "0 - Sair \n";
        std::cout << "Escolha uma opcao: ";
        std:: cin >> option;
        switch (option)
        {
            case 1:
            {
                int virtualAddress;

                std::cout << "Informe o endereco virtual: ";
                std::cin >> virtualAddress;

                if (virtualAddress >= 0 && virtualAddress < VirtualMemorySize)
                {
                    int pageNumber = GetPageNumber(virtualAddress);
                    int offset = GetPageOffset(virtualAddress);
                    int frameNumber = PageTable[pageNumber];

                    std::cout << "Endereco virtual: 00000000V" << std::hex << virtualAddress << std::dec << '\n';
                    std::cout << "pageNumber: " << pageNumber << '\n';
                    std::cout << "offset: " << offset << '\n';

                    if (frameNumber == -1)
                    {
                        PageFaultCount++;
                        std:: cout << "Ocorreu Page fault procurando fora da ram.. \n";
                        bool loaded = LoadPage(pageNumber);
                        if (loaded == false)
                        {
                            ReplaceOldestPage(pageNumber);
                        }
                        frameNumber = PageTable[pageNumber];
                    }

                    int physicalAddress = GetPhysicalAddress(frameNumber, offset);

                    std::cout << "frameNumber: " << frameNumber << '\n';
                    std::cout << "Endereco fisico: 00000000E" << std::hex << physicalAddress << std::dec << '\n';
                }
                else
                {
                    std::cout << "Endereco virtual invalido \n";
                }
                break;
            }

            case 2:
            {
                int physicalAddress;

                std::cout << "Informe o endereco fisico: ";
                std::cin >> physicalAddress;
                if (physicalAddress >= 0 && physicalAddress < PhysicalMemorySize)
                {
                    int frameNumber = GetFrameNumber(physicalAddress);
                    int offset = GetPageOffset(physicalAddress);
                    int pageNumber = GetVirtualPage(frameNumber);

                    std::cout << "Endereco fisico: 00000000E" << std::hex << physicalAddress << std::dec << '\n';

                    std::cout << "frameNumber: " << frameNumber << '\n';
                    std::cout << "offset: " << offset << '\n';

                    if (pageNumber == -1)
                    {
                        std::cout << "Quadro sem pagina virtual correspondente.\n";
                    }
                    else
                    {
                        int virtualAddress = GetVirtualAddress(pageNumber, offset);

                        std::cout << "pageNumber: " << pageNumber << '\n';
                        std::cout << "Endereco virtual: 00000000V" << std::hex << virtualAddress << std::dec << '\n';
                    }
                }
                else
                {
                    std::cout << "Endereco fisico invalido.\n";
                }
                break;
            }

            case 3:
            {
                int virtualAddress = 600;

                int pageNumber = GetPageNumber(virtualAddress);
                int offset = GetPageOffset(virtualAddress);
                int frameNumber = PageTable[pageNumber];

                if (frameNumber == -1)
                {
                    std::cout << "Page Fault! Carregando pagina...\n";

                    if (!LoadPage(pageNumber))
                    {
                        ReplaceOldestPage(pageNumber);
                    }
                    frameNumber = PageTable[pageNumber];
                }

                int physicalAddress = GetPhysicalAddress(frameNumber, offset);

                std::cout << "Endereco virtual: " << virtualAddress << '\n';
                std::cout << "Endereco fisico: " << physicalAddress << '\n';
                std::cout << "Valor armazenado: "  << static_cast<int>(RAM[physicalAddress]) << '\n';

                break;
            }
            case 4 :
            {
                std:: cout << PageFaultCount << '\n';
                break;
            }
 
            case 0:
            {
                std::cout << "Encerrando simulador  \n";
                menu = false;
                break;
            }

            default:
            {
                std::cout << "Opcao invalida.\n";
                break;
            }
        }
    }

    return 0;
}
