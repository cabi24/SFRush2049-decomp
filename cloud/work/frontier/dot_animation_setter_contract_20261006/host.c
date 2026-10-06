/* Executes the unchanged caller and actual accepted setter as host C89. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "group/caller.c"

typedef struct IndexedRecord44 { u8 unknown00[20]; u16 value; u8 unknown16[46]; } IndexedRecord44;
IndexedRecord44 D_8012E700[8];
s32 D_801170FC;
volatile f32 D_8002EB94;
Kind D_80117530[8];
u16 D_801427C0[256];
static Obj node;
static Model model;
static int mutation, call_count, calls[2][6];
static unsigned int float_bits(float value) { unsigned int out;memcpy(&out,&value,4);return out; }
static float from_bits(unsigned int value) { float out;memcpy(&out,&value,4);return out; }
static void record(int kind) {
    int *row;
    assert(call_count<2);
    row=calls[call_count++];row[0]=kind;row[1]=model.id;row[2]=node.step;
    row[3]=(int)float_bits(node.timer);row[4]=model.frame;row[5]=mutation;
}
void entity_spawn_callback(s16 id,s32 arg1,s32 arg2) {
    assert(id==model.id && arg1==0 && arg2==0);record(2);
    if(mutation) {node.step=1234;model.frame=222;D_801170FC=456;D_8002EB94=.5f;D_8012E700[7].value=0xface;}
}
void entity_transform_apply(void *p,s32 unlink) {
    assert(p==&node && unlink==1);record(1);
    if(mutation) {node.timer=.25f;D_8012E700[0].value=0xbeef;}
}
int main(void) {
    int mode,hold,step,count,base,frame,flags,id,type,i,j;
    unsigned int timer,delta;
    Obj before_node;Model before_model;Kind before_kind[8];
    IndexedRecord44 before_resources[8];u16 before_map[256];
    while(scanf("%d %d %d %u %u %d %d %d %d %d %d %d",&mode,&hold,&step,&timer,&delta,&count,&base,&frame,&flags,&id,&type,&mutation)==12) {
        memset(&node,0xa5,sizeof(node));memset(&model,0xa5,sizeof(model));
        memset(D_80117530,0xa5,sizeof(D_80117530));memset(D_8012E700,0xa5,sizeof(D_8012E700));
        node.model=&model;node.step=(s16)step;node.timer=from_bits(timer);
        model.id=(s16)id;model.type=(s16)type;model.frame=(s16)frame;model.base=(s16)base;model.count=(s16)count;
        D_801170FC=hold;D_8002EB94=from_bits(delta);
        for(i=0;i<8;i++)D_80117530[i].flags=(u16)(i==type?flags:0xa55a);
        for(i=0;i<256;i++)D_801427C0[i]=(u16)(i*257+0x1234);
        before_node=node;before_model=model;memcpy(before_kind,D_80117530,sizeof(before_kind));
        memcpy(before_resources,D_8012E700,sizeof(before_resources));memcpy(before_map,D_801427C0,sizeof(before_map));
        call_count=0;memset(calls,0,sizeof(calls));
        audio_channel_setup(&node,(s16)mode);
        printf("%d %u %d %d %u",node.step,float_bits(node.timer),model.frame,D_801170FC,float_bits(D_8002EB94));
        for(i=0;i<8;i++)printf(" %u",(unsigned int)D_8012E700[i].value);
        printf(" %d",call_count);
        for(i=0;i<2;i++)for(j=0;j<6;j++) {
            if(j==3)printf(" %u",(unsigned int)calls[i][j]);else printf(" %d",calls[i][j]);
        }
        putchar('\n');
        /* All untouched storage, including pointer fields and object gaps. */
        node.step=before_node.step;node.timer=before_node.timer;model.frame=before_model.frame;
        assert(memcmp(&node,&before_node,sizeof(node))==0);assert(memcmp(&model,&before_model,sizeof(model))==0);
        for(i=0;i<8;i++)D_8012E700[i].value=before_resources[i].value;
        assert(memcmp(D_8012E700,before_resources,sizeof(before_resources))==0);
        assert(memcmp(D_80117530,before_kind,sizeof(before_kind))==0);assert(memcmp(D_801427C0,before_map,sizeof(before_map))==0);
    }
    return 0;
}
