/* Executes the unchanged candidate as strict-aliasing C89 with UBSan/bounds. */
#include <assert.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "candidate.c"
#endif
#include CANDIDATE
ResourceCache D_803BA230[16];
s16 D_80142B08[13];
static s32 output_value, return_handle, mutation, call_count;
static unsigned char snapshots[2][222];
static s32 arguments[2][6];
static s32 expected_id;
static unsigned int u32read(const unsigned char *p) {
    return ((unsigned int)p[0]<<24)|((unsigned int)p[1]<<16)|((unsigned int)p[2]<<8)|p[3];
}
static void u32write(unsigned char *p,unsigned int x) {
    p[0]=(unsigned char)(x>>24);p[1]=(unsigned char)(x>>16);p[2]=(unsigned char)(x>>8);p[3]=(unsigned char)x;
}
static void capture(unsigned char *p) {
    s32 i;unsigned short u;
    for(i=0;i<16;i++) {
        u32write(p+i*12,(unsigned int)D_803BA230[i].handle);
        p[i*12+4]=(unsigned char)D_803BA230[i].loaded;
        p[i*12+5]=(unsigned char)D_803BA230[i].id;
        memcpy(p+i*12+6,D_803BA230[i].reserved,6);
    }
    for(i=0;i<13;i++){u=(unsigned short)D_80142B08[i];p[192+i*2]=(unsigned char)(u>>8);p[193+i*2]=(unsigned char)u;}
    u32write(p+218,(unsigned int)output_value);
}
static void alter(s32 which) {
    s32 i;
    if(!mutation)return;
    for(i=0;i<16;i++)if(D_803BA230[i].id==expected_id)break;
    assert(i<16);i=(i+7)%16;
    if(which==1){D_803BA230[i].handle=0x12345678;D_803BA230[i].loaded=-128;}
    else {D_803BA230[i].id=-3;output_value=0x76543210;}
}
s32 audio_frame_sync(s32 kind,s32 skip,s32 async,s32 flag,void *buffer) {
    assert(call_count==0 && buffer==0);
    arguments[0][0]=1;arguments[0][1]=kind;arguments[0][2]=skip;arguments[0][3]=async;arguments[0][4]=flag;arguments[0][5]=0;
    capture(snapshots[0]);call_count++;alter(1);return return_handle;
}
void func_800BB02C(s32 slot,s32 kind,void *data) {
    assert(call_count==1 && data==0);
    arguments[1][0]=2;arguments[1][1]=slot;arguments[1][2]=kind;arguments[1][3]=0;
    capture(snapshots[1]);call_count++;alter(2);
}
s32 host_run(const unsigned char *initial,s32 id,s32 kind,s32 handle,s32 mode,unsigned char *final,unsigned char *events,s32 *args) {
    s32 i,result;unsigned int u;unsigned short v;
    assert(id>=0 && id<13 && kind>=-32768 && kind<=32767);
    for(i=0;i<16;i++) {
        u=u32read(initial+i*12);memcpy(&D_803BA230[i].handle,&u,4);
        memcpy(&D_803BA230[i].loaded,initial+i*12+4,1);memcpy(&D_803BA230[i].id,initial+i*12+5,1);
        memcpy(D_803BA230[i].reserved,initial+i*12+6,6);
    }
    for(i=0;i<13;i++){v=(unsigned short)(((unsigned short)initial[192+i*2]<<8)|initial[193+i*2]);memcpy(&D_80142B08[i],&v,2);}
    u=u32read(initial+218);memcpy(&output_value,&u,4);
    return_handle=handle;mutation=mode;expected_id=id;call_count=0;
    memset(snapshots,0,sizeof(snapshots));memset(arguments,0,sizeof(arguments));
    result=func_80390BC0((s16)id,(s16)kind,&output_value);
    capture(final);memcpy(events,snapshots,sizeof(snapshots));memcpy(args,arguments,sizeof(arguments));
    args[12]=call_count;return result;
}
