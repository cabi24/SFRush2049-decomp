#include <stdio.h>
#include <string.h>
#include <stddef.h>
#ifndef CANDIDATE_PATH
#define CANDIDATE_PATH "../../matches/ovl_b/func_8038A8CC.c"
#endif
#include CANDIDATE_PATH
RingState D_80399A70;
s16 D_80399AD8;
char D_80394D14[] = "fixture texture";
volatile u8 D_80140BDC;
static int objects[25];
static RingState before;
static unsigned count_value, variant;
static int calls, failed;
static void fail(const char *why) { fprintf(stderr,"mismatch: %s\n",why); failed=1; }
NameEntry *func_800B24EC(char *name, s16 *out, s8 lo, s8 hi, s32 err)
{
    unsigned i;
    RingState expected;
    int high = ((int)count_value - 1) & 255;
    if (high >= 128) high -= 256;
    calls++;
    if (name != D_80394D14 || out != &D_80399AD8 || lo != 0 || hi != high || err != 1)
        fail("helper arguments");
    memcpy(&expected,&before,sizeof(expected)); expected.wrapped=0; expected.next=0;
    if (memcmp(&D_80399A70,&expected,sizeof(expected))) fail("helper snapshot");
    D_80399A70.wrapped=(s8)(variant-8);
    D_80399A70.unknown01=(u8)(variant*17+3);
    D_80399A70.next=(u16)(variant*4097);
    for(i=0;i<25;i++) D_80399A70.objects[i]=&objects[(i+variant)%25];
    *out=(s16)(variant*4099);
    D_80140BDC=(u8)(count_value+113);
    return (NameEntry *)0;
}
int main(void)
{
    unsigned i;
    RingState expected;
    for(count_value=0;count_value<256;count_value++) {
        for(variant=0;variant<16;variant++) {
            memset(&D_80399A70,(int)(count_value+variant),sizeof(D_80399A70));
            D_80399A70.wrapped=(s8)(variant-7);
            D_80399A70.next=(u16)(count_value*257);
            for(i=0;i<25;i++) D_80399A70.objects[i]=&objects[i];
            memcpy(&before,&D_80399A70,sizeof(before));
            D_80399AD8=(s16)0x1234;
            D_80140BDC=(u8)count_value;
            calls=0;
            func_8038A8CC();
            memcpy(&expected,&before,sizeof(expected));
            expected.wrapped=(s8)(variant-8);
            expected.unknown01=(u8)(variant*17+3);
            expected.next=(u16)(variant*4097);
            for(i=0;i<25;i++) expected.objects[i]=(void *)0;
            if(calls!=1 || D_80399AD8!=(s16)(variant*4099) ||
               D_80140BDC!=(u8)(count_value+113) ||
               memcmp(&D_80399A70,&expected,sizeof(expected))) fail("final state");
            if(failed) return 1;
        }
    }
    puts("4096 complete-state host cases passed");
    return 0;
}
