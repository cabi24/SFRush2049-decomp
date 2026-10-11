/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800E7A98 (0x800E7A98, 172 bytes): destroy a game heap. Under the heap lock (osRecvMesg /
 * osJamMesg on D_80152770) it takes heap, or the default (first) heap D_801527C8 when heap is NULL,
 * unlinks it from the heap list that starts at D_801527C8 (p->next == heap -> p->next = heap->next),
 * and frees its memory with audio_reverb_update(address, 1) (the heap-family free: address in a1,
 * tag in a2 by IPA; it uses s0/s1 unsaved, which is why this function saves them). No arcade ancestor.
 *
 * w15d: the "default heap" choice is the static helper heap_or_default (same body as the one in the
 * locked src/blob/sound_play_menu.c); umerge inlines it. Its return value is the web that holds h (v0)
 * and the assignment `h = heap_or_default(heap)` is the separate web starting at the join that retail
 * keeps in a1 (`move a1,v0`), which every hand-written copy spelling lost to copy propagation (w11a).
 * SHAPING DEVICE (disclosed): the compiled-out `if (heap == NULL) {}` before the lock (w11a's draft had it
 * too). It adds a use of the parameter before osRecvMesg, so the param web is coloured (a3, saved in its
 * home across the call: retail's `move a3,a0; sw a3,32(sp); lw a3,32(sp)`). `if (heap) {}` right after the
 * lock also matches. Without it the param is not coloured (`sw a0,32(sp)` / `lw v1,32(sp)`: 5 words).
 * Only EQUAL in the whole-program unit / this group (audio_reverb_update must be internal).
 */
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

static Heap *heap_or_default(Heap *heap)
{
    if (heap != 0) {
        return heap;
    }
    return D_801527C8;
}

void func_800E7A98(Heap *heap)
{
    Heap *p;
    Heap *h;

    if (heap == NULL) {
    }
    osRecvMesg(&D_80152770, NULL, 1);
    h = heap_or_default(heap);
    for (p = D_801527C8; p != NULL; p = p->next) {
        if (p->next == h) {
            p->next = h->next;
            break;
        }
    }
    audio_reverb_update((u32)h, 1);
    osJamMesg(&D_80152770, NULL, 0);
}
