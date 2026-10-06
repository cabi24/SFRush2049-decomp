/* Compile the unchanged submitted C with allocation/queue contract hooks. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "group.c"

s32 D_80142728, D_801427A8;
f32 D_80124280, D_801247EC;
s8 D_80146115, D_8010FFC0;
static unsigned char *allocation, *record;
static int stage, mutation;
static unsigned char trace[48];
static void canonicalize(unsigned char *p);

s32 osRecvMesg(OSMesgQueue *q, void *msg, s32 block) {
    assert(stage++ == 0 && (void *)q == &D_80142728 && msg == 0 && block == 1);
    return 0;
}
void *func_80091B00(void) {
    assert(stage++ == 1);
    return record;
}
s32 osJamMesg(OSMesgQueue *q, void *msg, s32 block) {
    int i;
    assert(block == 0);
    if (stage == 2) {
        assert((void *)q == &D_80142728 && msg == 0);
        memcpy(trace, record, 24);
        canonicalize(trace);
        canonicalize(record);
        if (mutation == 1) for (i=0;i<24;i++) record[i] ^= 0xA5;
        if (mutation == 2) {
            int indexes[7] = {2,4,7,8,11,12,13};
            for (i=0;i<7;i++) record[indexes[i]] ^= 0x5A;
        }
        canonicalize(record);
    } else {
        assert(stage == 3 && (void *)q == &D_801427A8 && msg == record);
        memcpy(trace+24,record,24);
        canonicalize(trace+24);
    }
    stage++;
    return 0;
}
static void canonicalize(unsigned char *p) {
    unsigned int one=1;
    int j;
    if (*(unsigned char *)&one) for(j=0;j<2;j++) {
        unsigned char x;
        x=p[4+j];p[4+j]=p[7-j];p[7-j]=x;
        x=p[8+j];p[8+j]=p[11-j];p[11-j]=x;
    }
}
int main(void) {
    unsigned int b,a,x,y;
    float blend,amount;
    int first,second,i;
    assert(sizeof(float)==4 && sizeof(int)==4);
    allocation=(unsigned char *)malloc(56);
    assert(allocation);record=allocation+16;
    while(scanf("%u %u %u %u %d",&b,&a,&x,&y,&mutation)==5) {
        memcpy(&blend,&b,4);memcpy(&amount,&a,4);memcpy(&first,&x,4);memcpy(&second,&y,4);
        memset(allocation,0xCC,56);
        for(i=0;i<24;i++) record[i]=(unsigned char)(i*37+11);
        stage=0;
        speed_set(blend,amount,first,second);
        assert(stage==4);
        for(i=0;i<16;i++) assert(allocation[i]==0xCC && allocation[40+i]==0xCC);
        for(i=0;i<48;i++) printf("%u%c",(unsigned int)trace[i],i==47?'\n':' ');
    }
    free(allocation);return 0;
}
