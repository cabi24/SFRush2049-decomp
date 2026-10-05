typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;
#define NULL ((void *)0)
typedef struct OSMesgQueue OSMesgQueue;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
void audio_reverb_update(u32 address, s32 tag);
extern OSMesgQueue D_80152770;

typedef struct Heap {
    u32 pad0;
    struct Heap *next;  /* 4 */
} Heap;
extern Heap *D_801527C8;

void func_800E7A98(Heap *heap)
{
    Heap *p;
    Heap *next;

    osRecvMesg(&D_80152770, NULL, 1);
    if (heap == NULL) {
        heap = D_801527C8;
    }
    p = D_801527C8;
    while (p != NULL) {
        next = p->next;
        if (heap == next) {
            p->next = heap->next;
            break;
        }
        p = next;
    }
    audio_reverb_update((u32)heap, 1);
    osJamMesg(&D_80152770, NULL, 0);
}
