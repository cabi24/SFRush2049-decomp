/* Execute unchanged C89 source; serialize logical pointers into native fixture
 * addresses. LP64 layouts are not offered as native ABI evidence. */
#include <stddef.h>
#include <string.h>
#ifndef CANDIDATE_PATH
#error CANDIDATE_PATH required
#endif
#include CANDIDATE_PATH
State D_80399AE0;
Object *D_80143FD8;
static Object objects[8];
static Slot slots[8];
static unsigned int rng_seed, initial_count, length, calls;
static unsigned int snapshots[5][8];
static unsigned int allocation_size, allocation_count;
static unsigned int float_bits(float value) { unsigned int b; memcpy(&b,&value,4); return b; }
static void snapshot(unsigned int *p) {
    unsigned int i;
    p[0]=float_bits(D_80399AE0.delay); p[1]=float_bits(D_80399AE0.phase_a);
    p[2]=float_bits(D_80399AE0.phase_b); p[3]=rng_seed;
    p[4]=0;p[5]=0;p[6]=(u8)D_80399AE0.count;p[7]=D_80143FD8?0x81000000U:0;
    for(i=0;i<8;i++) {
        unsigned int j,ptr=0;
        for(j=0;j<8;j++)if(slots[i].object==&objects[j])ptr=0x81000000U+j*16;
        p[5]=p[5]*33U+ptr;
        p[5]=p[5]*33U+(unsigned short)slots[i].active;
        p[5]=p[5]*33U+(unsigned short)slots[i].count;
    }
}
void *audio_dma_sync(Heap *heap, u32 size) {
    if(heap!=0) allocation_count=1000;
    allocation_count++;
    allocation_size=size;
    snapshot(snapshots[0]);
    D_80143FD8=length ? &objects[0] : 0;
    D_80399AE0.count=(s8)((initial_count+73)&255);
    return slots;
}
f32 func_8008B2E4(f32 range) {
    float result;
    unsigned int k;
    if(calls>=4) return -1000.0f;
    snapshot(snapshots[calls+1]);
    snapshots[calls+1][4]=float_bits(range);
    rng_seed=rng_seed*0x41c64e6dU+12345U;
    k=(rng_seed>>16)&32767;
    result=(float)k*range;
    result=result/32768.0f;
    calls++;
    return result;
}
int run_host(unsigned int raw_count, unsigned int var, unsigned int seed, int reuse,
             unsigned int *out) {
    unsigned int i,j,n,selected=0;
    unsigned char before_objects[sizeof(objects)], before_slots[sizeof(slots)];
    unsigned char before_state[sizeof(State)];
    length=var%9; initial_count=raw_count; rng_seed=seed;
    calls=0; allocation_count=0; allocation_size=0;
    memset(snapshots,0,sizeof(snapshots));
    memset(&D_80399AE0,0xa5,sizeof(D_80399AE0));
    D_80399AE0.count=(s8)raw_count;
    D_80399AE0.delay=-17.0f;D_80399AE0.phase_a=23.0f;D_80399AE0.phase_b=-31.0f;
    D_80399AE0.slots=reuse?slots:0;
    for(i=0;i<8;i++) {
        memset(&objects[i],0x59,sizeof(Object));
        objects[i].next=(i+1<length)?&objects[i+1]:0;
        objects[i].flags=(u8)(raw_count+i*43+var*11);
        memset(&slots[i],0x7b,sizeof(Slot));slots[i].object=0;
        slots[i].active=-123;slots[i].count=234;
    }
    D_80143FD8=reuse && length ? &objects[0] : 0;
    memcpy(before_objects,objects,sizeof(objects));memcpy(before_slots,slots,sizeof(slots));
    memcpy(before_state,&D_80399AE0,sizeof(State));
    func_8039133C(reuse);
    if(calls!=4 || allocation_count!=(reuse==0) ||
       (reuse==0 && allocation_size != (unsigned int)((int)(s8)raw_count*8)))return 1;
    if(memcmp(before_objects,objects,sizeof(objects)))return 2;
    for(i=0;i<length;i++)if(objects[i].flags&32)selected++;
    for(i=selected;i<8;i++)if(memcmp(before_slots+i*sizeof(Slot),&slots[i],sizeof(Slot)))return 3;
    for(i=0;i<sizeof(State);i++) {
        int touched = i==offsetof(State,count) && !reuse;
        touched |= i>=offsetof(State,delay) && i<offsetof(State,phase_b)+4;
        touched |= !reuse && i>=offsetof(State,slots) && i<offsetof(State,slots)+sizeof(void*);
        if(!touched && ((unsigned char *)&D_80399AE0)[i]!=before_state[i])return 4;
    }
    n=0;out[n++]=(u8)D_80399AE0.count;
    out[n++]=float_bits(D_80399AE0.delay);out[n++]=float_bits(D_80399AE0.phase_a);
    out[n++]=float_bits(D_80399AE0.phase_b);out[n++]=rng_seed;out[n++]=allocation_count;
    out[n++]=allocation_size;
    for(i=0;i<8;i++) {
        unsigned int p=0;
        if(slots[i].object) {
            for(j=0;j<8;j++)if(slots[i].object==&objects[j])p=0x81000000U+j*16;
            if(!p)return 5;
        }
        out[n++]=p;out[n++]=(unsigned short)slots[i].active;out[n++]=(unsigned short)slots[i].count;
    }
    for(i=0;i<5;i++)for(j=0;j<8;j++)out[n++]=snapshots[i][j];
    return 0;
}
