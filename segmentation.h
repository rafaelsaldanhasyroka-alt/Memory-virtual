#ifndef SEGMENTATION_H
#define SEGMENTATION_H
struct segment_simulation {
    unsigned int id;
    unsigned int size;
};
struct memory_position
{
    bool validBit;
    bool dirtyBit;
    unsigned int attached_process;
};

int allocSegment(segment_simulation segment, memory_position RAM);
int deallocSegment(segment_simulation segment, memory_position RAM);
void defragMemory(memory_position* memory);
void routine();
#endif