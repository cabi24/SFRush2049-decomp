/* Test harness only. Includes the unchanged candidate supplied by the verifier. */
#include <stdio.h>
#include <string.h>
#ifndef CANDIDATE
#error CANDIDATE must name the exact candidate
#endif
#include CANDIDATE
s32 state_word_a, D_801170FC, gameplay_mode;
f32 D_80123DE0, D_80116178;
volatile f32 D_8002EB94;
static unsigned int mask, next_mode, next_accum, next_delta, events[6], event_count;
static unsigned int bits(float value) { unsigned int word; memcpy(&word,&value,4); return word; }
static float value(unsigned int word) { float result; memcpy(&result,&word,4); return result; }
static void record(unsigned int kind) {
    unsigned int i=event_count++ * 3;
    events[i]=kind; events[i+1]=(unsigned int)gameplay_mode; events[i+2]=bits(D_80116178);
}
void particles_update(s32 arg) {
    if(arg!=0) { fputs("bad argument\n",stderr); return; }
    record(1);
    if(mask&1)gameplay_mode=(s32)next_mode;
    if(mask&2)D_80116178=value(next_accum);
    if(mask&4)D_8002EB94=value(next_delta);
}
void func_80391B00(void) { record(2); }
int main(void) {
    unsigned int state,pause,mode,delta,accum,i;
    while(scanf("%u %u %u %u %u %u %u %u %u",&state,&pause,&mode,&delta,&accum,
                &mask,&next_mode,&next_accum,&next_delta)==9) {
        state_word_a=(s32)state;D_801170FC=(s32)pause;gameplay_mode=(s32)mode;
        D_8002EB94=value(delta);D_80116178=value(accum);D_80123DE0=value(0x40490fdbu);
        event_count=0;memset(events,0,sizeof(events));func_800B7FF8();
        printf("%u %u %u",bits(D_80116178),(unsigned int)gameplay_mode,event_count);
        for(i=0;i<6;i++)printf(" %u",events[i]);
        putchar('\n');
    }
    return 0;
}
