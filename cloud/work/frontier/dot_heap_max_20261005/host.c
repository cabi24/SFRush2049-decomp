/* Unchanged candidate, host-width pointers, C89 + UBSan differential harness. */
#include <stdio.h>
#include <string.h>
#include <assert.h>
struct OSMesgQueue { unsigned int words[6]; };
#include "group/heap_max.c"

OSMesgQueue D_80152770;
Heap *D_801527C8;
static Heap heaps[2];
static Block blocks[2][64];
static int mode, flags, count, call_count;

static void hook(int receive)
{
    Heap *selected;
    if (receive) {
        if (flags & 1) D_801527C8 = &heaps[1];
        if ((flags & 2) && count) {
            selected = mode < 2 ? &heaps[mode] : D_801527C8;
            selected->first->size = 0xFFFFFFFFU;
            selected->first->used = 0;
        }
    } else {
        D_801527C8 = &heaps[0];
        if (count) {
            selected = mode < 2 ? &heaps[mode] : &heaps[(flags & 1) != 0];
            selected->first->size = 0;
            selected->first->used = 1;
        }
    }
}

s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 blocking)
{
    assert(queue == &D_80152770 && message == 0 && blocking == 1);
    assert(call_count++ == 0);
    hook(1);
    return 0;
}

s32 osJamMesg(OSMesgQueue *queue, void *message, s32 blocking)
{
    assert(queue == &D_80152770 && message == 0 && blocking == 0);
    assert(call_count++ == 1);
    hook(0);
    return 0;
}

int main(void)
{
    unsigned int sizes[64], result;
    int used[64], order[64], i, h, j, next, scan;
    Block expected[2][64];
    Heap expected_heaps[2];
    OSMesgQueue expected_queue;
    while ((scan = scanf("%d%d%d", &count, &mode, &flags)) == 3) {
        assert(count >= 0 && count <= 64 && mode >= 0 && mode <= 2);
        memset(blocks, 0xB7, sizeof(blocks));
        memset(heaps, 0xA6, sizeof(heaps));
        memset(&D_80152770, 0x97, sizeof(D_80152770));
        for (i = 0; i < count; ++i) assert(scanf("%u", &sizes[i]) == 1);
        for (i = 0; i < count; ++i) assert(scanf("%d", &used[i]) == 1);
        for (i = 0; i < count; ++i) assert(scanf("%d", &order[i]) == 1);
        for (h = 0; h < 2; ++h) {
            heaps[h].first = count ? &blocks[h][order[h ? count - 1 : 0]] : 0;
            for (i = 0; i < count; ++i) {
                j = h ? count - 1 - i : i;
                next = h ? j - 1 : j + 1;
                blocks[h][order[j]].next = i + 1 < count ? &blocks[h][order[next]] : 0;
                blocks[h][i].size = sizes[i]; blocks[h][i].used = (s8)used[i];
            }
        }
        D_801527C8 = &heaps[0];
        /* Compute the only authorized external mutations on a snapshot. */
        hook(1); hook(0);
        memcpy(expected, blocks, sizeof(blocks));
        memcpy(expected_heaps, heaps, sizeof(heaps));
        memcpy(&expected_queue, &D_80152770, sizeof(expected_queue));
        for (h = 0; h < 2; ++h) for (i = 0; i < count; ++i) {
            blocks[h][i].size = sizes[i]; blocks[h][i].used = (s8)used[i];
        }
        D_801527C8 = &heaps[0]; call_count = 0;
        result = func_800E79F8(mode < 2 ? &heaps[mode] : 0);
        assert(call_count == 2 && D_801527C8 == &heaps[0]);
        assert(memcmp(expected, blocks, sizeof(blocks)) == 0);
        assert(memcmp(expected_heaps, heaps, sizeof(heaps)) == 0);
        assert(memcmp(&expected_queue, &D_80152770, sizeof(expected_queue)) == 0);
        printf("%u\n", result);
    }
    assert(scan == EOF);
    return 0;
}
