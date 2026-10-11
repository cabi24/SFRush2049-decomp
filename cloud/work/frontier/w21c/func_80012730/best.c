/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "types.h"
typedef struct AudioCacheNode { struct AudioCacheNode *next; struct AudioCacheNode *previous; unsigned int unknown08; unsigned int unknown0C; unsigned int unknown10; unsigned char *buffer; } AudioCacheNode;
typedef struct DmaEntry_80012730 { int length; unsigned int source; unsigned char *destination; } DmaEntry_80012730;
typedef struct Fetch_80012730 { unsigned char *first; unsigned char *second; short first_length; short second_length; } Fetch_80012730;
extern AudioCacheNode *D_80038354;
extern AudioCacheNode *D_80038358;
extern void *D_8003802C;
extern unsigned int D_80038038;
extern unsigned int D_8003803C;
extern unsigned short D_80038028;
extern AudioCacheNode *func_80012660(unsigned int tag);
extern unsigned int func_8002C5E0(unsigned int, unsigned int);
void func_80012730(Fetch_80012730 *fetch, unsigned int addr, unsigned int length, unsigned int tag)
{
    unsigned int size;
    AudioCacheNode *previous;
    AudioCacheNode *node;
    unsigned char *pointers[2];
    unsigned int lengths[2];
    unsigned int starts[2];
    unsigned short count;
    unsigned int offset;
    unsigned int end;
    unsigned int low;

    end = addr + length;
    low = addr - 0x600;
    count = 0;
    for (node = D_80038358; node != NULL; node = previous) {
        previous = node->previous;
        if (tag != node->unknown0C || node->unknown08 >= end || low >= node->unknown08) {
            continue;
        }
        if (node->unknown10 != 0x600 && node->unknown08 + node->unknown10 < addr + length) {
            if (addr + length >= node->unknown08 + 0x600) {
                size = 0x600 - node->unknown10;
            } else {
                size = (addr + length - node->unknown08 - node->unknown10 + 0xFF) & ~0xFF;
            }
            D_80038038 += 12;
            D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
            ((DmaEntry_80012730 *)D_8003802C)[D_80038028].source = node->unknown08 + node->unknown10;
            ((DmaEntry_80012730 *)D_8003802C)[D_80038028].length = size;
            ((DmaEntry_80012730 *)D_8003802C)[D_80038028].destination = node->buffer + node->unknown10;
            D_80038028++;
            node->unknown10 += size;
        }
        if (node != D_80038358) {
            if (node->next != NULL) {
                node->next->previous = node->previous;
            }
            if (node->previous != NULL) {
                node->previous->next = node->next;
            } else {
                D_80038354 = node->next;
            }
            D_80038358->next = node;
            node->previous = D_80038358;
            node->next = NULL;
            D_80038358 = node;
        }
        if (addr < node->unknown08) {
            pointers[count] = node->buffer;
            starts[count] = node->unknown08;
            lengths[count] = length - node->unknown08 + addr;
            length -= lengths[count];
            count++;
        } else if (node->unknown08 + node->unknown10 < addr + length) {
            pointers[count] = node->buffer + addr - node->unknown08;
            starts[count] = addr;
            lengths[count] = node->unknown08 + node->unknown10 - addr;
            length -= lengths[count];
            count++;
            addr = node->unknown08 + node->unknown10;
        } else {
            pointers[count] = node->buffer + addr - node->unknown08;
            starts[count] = addr;
            lengths[count] = length;
            length = 0;
            count++;
            break;
        }
    }
    if (length != 0) {
        offset = (addr - tag) % 1536;
        do {
            node = func_80012660(tag);
            node->unknown0C = tag;
            node->unknown08 = addr - offset;
            if (length + offset > 0x600) {
                node->unknown10 = 0x600;
                D_80038038 += 12;
            D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
                addr = addr - offset + 0x600;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].source = node->unknown08;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].length = 0x600;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].destination = node->buffer;
                D_80038028++;
                pointers[count] = node->buffer + offset;
                starts[count] = node->unknown08 + offset;
                lengths[count] = 0x600 - offset;
                length -= 0x600 - offset;
                count++;
            } else {
                node->unknown10 = (length + offset + 0xFF) & ~0xFF;
                D_80038038 += 12;
            D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].source = node->unknown08;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].length = node->unknown10;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].destination = node->buffer;
                D_80038028++;
                pointers[count] = node->buffer + offset;
                starts[count] = node->unknown08 + offset;
                lengths[count] = length;
                length = 0;
                count++;
            }
            offset = 0;
        } while (length != 0);
    }
    if (count == 1) {
        fetch->first = pointers[0];
        fetch->first_length = lengths[0];
        fetch->second_length = 0;
        return;
    }
    if (starts[0] < starts[1]) {
        fetch->first = pointers[0];
        fetch->second = pointers[1];
        fetch->first_length = lengths[0];
        fetch->second_length = lengths[1];
        return;
    }
    fetch->first = pointers[1];
    fetch->second = pointers[0];
    fetch->first_length = lengths[1];
    fetch->second_length = lengths[0];
}
