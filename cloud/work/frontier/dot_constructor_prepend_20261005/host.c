/* Host caller-contract hooks. Compile candidate.c unchanged as a separate TU. */
#include <stdint.h>
#include <string.h>
#include <stddef.h>
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Object {u32 link0,link4,handle;u8 pad12,active,field14,pad15;float start,value;u8 field24,pad25[3];u32 args[4],argument3,argument2;u8 kind,pad53[3];int end;} Object;
typedef struct List {u8 indirect,doubly,reserved[2];u32 count,head,tail;} List;
typedef char object_size[(sizeof(Object)==60)?1:-1];
typedef char args_offset[(offsetof(Object,args)==28)?1:-1];
typedef char head_offset[(offsetof(List,head)==8)?1:-1];
u8 D_80142728[24];
float D_80152748;
List D_80149860;
static Object object;
static u32 values[32];
static int phase,bad;
static u8 observed[60];
extern u32 car_stats_display(int,float,u32,u32,int,...);
static void big(u8 *p,u32 v) {p[0]=(u8)(v>>24);p[1]=(u8)(v>>16);p[2]=(u8)(v>>8);p[3]=(u8)v;}
static void normalized(u8 *out) {
    int i;u32 v;
    memcpy(out,&object,60);
    for(i=0;i<4;i++)big(out+28+4*i,object.args[i]);
    big(out+8,object.handle);memcpy(&v,&object.start,4);big(out+16,v);
    memcpy(&v,&object.value,4);big(out+20,v);
    big(out+44,object.argument3);big(out+48,object.argument2);big(out+56,(u32)object.end);
}
int osRecvMesg(void *q,void *m,int block) {
    if(q!=D_80142728 || m!=0 || block!=1 || phase!=0)bad=1;
    phase=1;memcpy(&D_80152748,&values[8],4);return 0;
}
Object *func_800D18D8(void) {if(phase!=1)bad=1;phase=2;return &object;}
void func_80091FBC(void *l,Object *o,u32 before) {
    if(l!=&D_80149860 || o!=&object || before!=D_80149860.head || phase!=2)bad=1;
    phase=3;normalized(observed);
    /* The allocator-result handle was initially opaque native-order bytes. */
    memcpy(observed+8,((u8 *)&object)+8,4);
    object.handle=values[9];
}
int osJamMesg(void *q,void *m,int block) {
    if(q!=D_80142728 || m!=0 || block!=0 || phase!=3 || object.active!=1)bad=1;
    phase=4;object.handle=values[10];return 0;
}
int run_case(u32 seed,int count,u8 *out,u8 *at_insert,u32 *result) {
    int i;float value;
    phase=bad=0;
    for(i=0;i<32;i++)values[i]=seed*0x9e3779b1U+(u32)i*0x1020304U;
    for(i=0;i<60;i++)((u8 *)&object)[i]=(u8)(seed*17U+(u32)i*31U);
    memset(&D_80149860,0,sizeof(D_80149860));
    D_80149860.head=seed%3==0?0:0x410000+4*(seed%17);
    D_80149860.tail=seed%3==0?0:0x420000+4*(seed%13);
    memcpy(&value,&values[1],4);
    *result=car_stats_display((int)values[0],value,values[2],values[3],count,
                             values[4],values[5],values[6],values[7]);
    normalized(out);memcpy(at_insert,observed,60);
    return bad || phase!=4;
}
