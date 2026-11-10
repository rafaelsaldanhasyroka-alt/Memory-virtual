#ifndef SEGMENTATION_H
#define SEGMENTATION_H
struct segment_simulation {

    bool valid_bit;
    unsigned int attached_process;
    unsigned int start;
    unsigned int end;
};
extern segment_simulation segments[16];

#endif