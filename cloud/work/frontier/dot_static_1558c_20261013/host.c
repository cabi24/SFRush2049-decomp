
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
static unsigned int seed = 0x12345678;
static unsigned int rnd(void) { seed = seed * 1664525u + 1013904223u; return seed; }

#include "../../../matches/boot_tail/func_8001558C.c"

u8 D_8002C630;
short D_80038390;
ResourceGroup *D_80038398[8];
static int locks, unlocks, calls, guarded, unlocked_expected;
static void *seen[5];
static u32 callback_result;
void func_80014594(void) { locks++; guarded++; }
void func_800145DC(void) { unlocks++; guarded--; }
u32 func_800178B0(Page_800178B0 *a, Page_800178B0 *b, MidiSetup_800178B0 *c, Arr_800178B0 *d, PlayPara_800178B0 *e) {
    assert(guarded == (unlocked_expected ? 0 : 1));
    seen[0]=a; seen[1]=b; seen[2]=c; seen[3]=d; seen[4]=e; calls++;
    return callback_result;
}
typedef union { unsigned int align; unsigned char bytes[1200]; } Blob;
static Blob blobs[5];
int main(void) {
    int test, b, j, n, expected, hit, unlocked;
    int success=0, guarded_hits=0, direct_hits=0;
    u16 id, key;
    ResourceHeader *h;
    Program *program, *selected;
    u32 stream, options;
    for (test=0; test<2400; test++) {
        memset(blobs, 0, sizeof(blobs));
        D_8002C630=(u8)(test % 7 != 0);
        D_80038390=(short)(test % 8 - 2);
        selected=0; hit=-1;
        id=(u16)((rnd()%5) | ((test & 16) ? 0x8000 : 0)); key=(u16)((rnd()%7) | ((test & 32) ? 0x8000 : 0)); unlocked=(test&1)?255:0;
        stream=0x12345678U; options=(test&2)?0x23456780U:0;
        for (b=0; b<5; b++) {
            h=(ResourceHeader *)blobs[b].bytes;
            h->id=(u16)((rnd()%5) | ((test & 16) ? 0x8000 : 0)); h->type=(u16)(rnd()%4 == 0);
            h->bank_a=40; h->bank_b=64; h->programs=96+(test&1);
            D_80038398[b]=(ResourceGroup *)h;
            program=(Program *)(blobs[b].bytes+104+(test&1)); n=(int)(rnd()%6);
            for(j=0;j<n;j++) program[j].id=(u16)((rnd()%7) | ((test & 32) ? 0x8000 : 0));
            program[n].id=0xffff;
        }
        if (D_8002C630) {
            for (b=0;b<D_80038390;b++) {
                h=(ResourceHeader *)D_80038398[b];
                if (h->id==id) {
                    if (!h->type) {
                        program=(Program *)((u8 *)h+h->programs+8);
                        for (j=0;program[j].id!=0xffff;j++) {
                            if (program[j].id==key) { selected=&program[j];hit=b;break; }
                        }
                    }
                    break;
                }
            }
        }
        locks=unlocks=calls=guarded=0; memset(seen,0,sizeof(seen));
        { static const u32 results[5]={0,0x12345678U,0x80000000U,0xdeadbeefU,0xffffffffU}; callback_result=results[test%5]; }
        expected=selected?(int)callback_result:-1; unlocked_expected=unlocked;
        assert(func_8001558C(id,key,stream,options,(u8)unlocked)==expected);
        assert(calls==(selected!=0));
        assert(locks==(selected && !unlocked)); assert(unlocks==locks); assert(!guarded);
        if (selected) {
            success++;if(unlocked)direct_hits++;else guarded_hits++;
            h=(ResourceHeader *)D_80038398[hit];
            assert(seen[0]==(u8 *)h+h->bank_a+8);
            assert(seen[1]==(u8 *)h+h->bank_b+8);
            assert(seen[2]==selected && seen[3]==(void *)(unsigned long)stream && seen[4]==(void *)(unsigned long)options);
        }
    }
    assert(success>0 && guarded_hits>0 && direct_hits>0);
    puts("8001558C: 2400 signed-count, high-ID, sentinel, first-bank, five-argument and guard controls passed");
    return 0;
}
