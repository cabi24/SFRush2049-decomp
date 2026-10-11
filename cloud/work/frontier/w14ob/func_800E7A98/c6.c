/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NOT A MATCH (w11a): 14 of 43 words differ in the unit, body length now 43 = retail (w10f best: 28 words,
 * 42-word body, 13 aligned rows). The loop is a goto loop (`p = next; loop: if (p) { next = p->next; if
 * (next == h) {...} else { p = next; goto loop; } }`), which gives retail's non-likely `bnez; move v1`
 * tail. Residual: retail keeps h in v0 and next in a0, with a separate a1 copy of h (`move a1,v0`) made
 * at the join before the loop. Ours colours h straight into a1 (the callee's IPA argument register) and
 * next into v0. Forcing h=v0, next=a0, p=v1 puts the a1 copy at the call instead of the join, so retail has a
 * second web (the argument) that starts at the join. Every copy-variable spelling is copy-propagated away.
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

void func_800E7A98(Heap *heap)
{
    Heap *h;
    Heap *next;
    Heap *p;
    osRecvMesg(&D_80152770, NULL, 1);
    h = (heap != NULL) ? heap : D_801527C8;
for (p = D_801527C8; p != NULL; p = p->next) {
        if (p->next == h) {
            p->next = h->next;
            break;
        }
    }
    audio_reverb_update((u32)h, 1);
    osJamMesg(&D_80152770, NULL, 0);
}
