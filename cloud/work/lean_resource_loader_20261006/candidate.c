/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
#ifndef LEAN_LOADER_H
#define LEAN_LOADER_H
#define NULL ((void *)0)
typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;
typedef struct ResSlot {
 s8 f0,loaded,f2,f3,f4,f5;u8 type,pad07;s32 pad08;u8 **p0C;u8 *w10;
} ResSlot;
typedef struct SubRec {u32 header[2];u8 *a,*b;} SubRec;
typedef struct ObjRec {u8 pad00[22];s16 nsub;SubRec sub[4];} ObjRec;
typedef struct TexRec {u8 pad00[24];u8 *w18,*w1C;u32 word20;} TexRec;
typedef struct PalRec {u8 pad00[20];u8 *w14;} PalRec;
typedef struct Tab {void *ptr;s32 count;} Tab;
typedef struct Table {void *header;s32 count;void *entries;} Table;
typedef struct OSMesgQueue {s32 words[6];} OSMesgQueue;
typedef struct OSThread OSThread;
typedef struct OSIoMesgHdr {u16 type;u8 pri,status;OSMesgQueue *retQueue;} OSIoMesgHdr;
typedef struct OSIoMesg {OSIoMesgHdr hdr;void *dramAddr;u32 devAddr,size;void *piHandle;} OSIoMesg;
extern ResSlot D_80156D38[];
extern s32 D_8002E580[],D_80123564[],D_80140BDC,D_801569AC,D_8002EB70;
extern u32 D_8011B5BC[];
extern s8 D_801234AC[],D_80152570,D_80140A04;
extern s16 active_player_count;
extern Tab D_801161F4[],D_80151AE8[],D_80138670[];
extern TexRec *D_801392D0;
extern s32 D_801392D4,D_80149B08;
extern OSMesgQueue D_80035410,D_80035440,D_80156BB0;
extern void *D_80156BD4;
extern OSThread D_80034690;
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
void osCreateMesgQueue(OSMesgQueue *,void **,s32);
void osCreateViManager(OSThread *,s32);
void osInvalICache_full(void *,s32);
void osInvalDCache(void *,s32);
s32 __osPiRawStartDma(OSIoMesg *,s32,s32,u32,void *,u32,OSMesgQueue *);
s32 lzss_decompress(void *,void *);
s32 inflate_decompress(void *,void *,s32);
void *audio_task_complete(void *,u32);
void *audio_dma_sync(s32,u32);
s32 audio_buffer_sync(u32);
void entity_render_mode(s32);
void func_80096C28(Table *,void *);
void func_80096BBC(Table *,s32,s32);
s32 lookup_with_output(Table *,s32,s32 *);
typedef struct ResourceGfx {u32 w0,w1;} ResourceGfx;
void entity_cull_check(ResourceGfx *,ResourceGfx *,s32);
void entity_lod_select(u32 *,s32,s32,s32,u32);
s32 func_80097694(s32,s8);
void func_800972C4(s32);
void func_80096288(s32,s32,s32);
void func_80096CA8(s32,s32);
void func_80097164(s32);
#endif

void func_80097164(s32 slot) {
    OSIoMesg message;
    ResSlot *resource;
    void *destination;
    s32 size;
    u32 source;
    s32 format;
    u8 type;
    resource=&D_80156D38[slot];
    type=resource->type;
    format=D_80123564[type];
    if (format==0) {
        if(resource->f5) destination=*resource->p0C;
        else destination=resource->p0C;
        size=D_8002E580[type];
        source=D_8011B5BC[type];
        osInvalICache_full(destination,size);
        osInvalDCache(destination,size);
        __osPiRawStartDma(&message,0,0,source,destination,size,&D_80035410);
        osRecvMesg(&D_80035410,0,1);
    } else if(format==1) {
        if(resource->f5) destination=*resource->p0C;
        else destination=resource->p0C;
        lzss_decompress((void *)D_8011B5BC[type],destination);
    } else {
        if(resource->f5) destination=*resource->p0C;
        else destination=resource->p0C;
        inflate_decompress((void *)D_8011B5BC[type],destination,1);
    }
    resource->f0=1;
    func_80096CA8(slot,1);
}
