#include "segmentation.h"
#include <iostream>
int allocSegment(segment_simulation segment, memory_position* memory)
{
    defragMemory(memory);
    for (int i = 0; i < 512; i++)
    {
        int n = 0, start;
        if (memory[i].validBit == 1)
        {
            n++;
        }
        else
        {
            if (n == segment.size)
            {
                for(int j = i-(n-1); j<n; j++)
                {
                    memory[i].validBit=0;
                    memory[i].attached_process=segment.id;
                }
            }
            else
            {
                std::cout<<"\n Segmentation Fault";
            }
        }
    }
}

int deallocSegment(segment_simulation segment, memory_position* memory)
{
    for (int i = 0; i < 65535; i++)
    {
        if (memory[i].attached_process == segment.id)
        {
            memory[i].validBit = 1;
        }
    }   
}
void defragMemory(memory_position* memory)
{
    for (int i = 0; i < 512; i++)
    {
        for (size_t j = 0; j < 512; j++)
        {
            if (memory[j].validBit > memory[j+1].validBit)
            {
                bool aux =  memory[j+1].validBit;
                memory[j+1].validBit = memory[j].validBit;
                memory[j].validBit = aux;
            }
        }
    }
}
