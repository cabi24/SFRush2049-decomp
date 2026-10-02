/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct { void *hdr; s32 other[4]; } Resource20;
typedef struct { u16 other,id; } Instrument4;
extern u8 D_80151960;
extern s8 D_8011EAA4;
extern s32 D_8011EAA0;
extern u8 D_80152750[],D_8015276C[];
extern u8 D_80035260[],D_800350B0[],D_80033EA0[],D_8002F660[];
extern Resource20 D_80156D44[];
extern Instrument4 D_8011F0A0[];
extern void *D_80152464,*D_801526D8,*D_80152690;
extern s32 D_801525FC;
extern s8 D_8011EAAC;
extern void task_complete_signal(void *);
extern void dma_wait_complete(void *);
extern void osCreateMesgQueue(void *,void *,s32);
extern void osCreateThread(void *,s32,void (*)(void *),void *,void *,s32);
extern void osStartThread(void *);
extern void tire_compound_set(void);
extern s32 audio_frame_sync(s32,s32,s32,s32,s32);
extern void display_list_alloc(s32);
extern void func_80096288(s32,s32,s32);
extern void func_80020598(void *);
extern void func_80020274(s32);
extern s32 func_800202c4(void);
extern void func_800154a4(void);
extern void func_800109c0(void);
extern s32 func_800108e0(s32,u8,u8,u8,s32,u32);
extern void func_80010840(s32,u8,u8,u8,s32,u32);
extern void func_8001536c(void *,u16,s32,void *,void *);
extern void func_8002043c(s32,s32,s32);
extern void func_80020494(s32,s32,s32,s32);
extern void func_800A4CB8(s32);
void car_shadow_render(s32 channels,s32 control,s32 enabled)
{
    u32 memory;
    s32 handle,i;
    Instrument4 *instrument;
    D_80151960=enabled<1;
    if(D_80151960) memory=0x80000;
    else memory=0x10000;
    if(!D_8011EAA4) {
        osCreateMesgQueue(D_80152750,D_8015276C,1);
        osCreateThread(D_80035260,3,task_complete_signal,0,D_80033EA0,10);
        osStartThread(D_80035260);
        osCreateThread(D_800350B0,9,dma_wait_complete,0,D_8002F660,9);
        osStartThread(D_800350B0);
        tire_compound_set();
        handle=audio_frame_sync(6,0,0,0,0);
        display_list_alloc(handle);
        func_80096288(handle,0,0);
        D_80152464=D_80156D44[handle].hdr;
        handle=audio_frame_sync(7,0,0,0,0);
        display_list_alloc(handle);
        func_80096288(handle,0,0);
        D_801526D8=D_80156D44[handle].hdr;
        handle=audio_frame_sync(8,0,0,0,0);
        display_list_alloc(handle);
        func_80096288(handle,0,0);
        D_80152690=D_80156D44[handle].hdr;
        D_801525FC=0x39b40;
        func_80020598(&D_8011EAAC);
    }
    if(D_8011EAA4) {
        func_80020274(22050);
        while(!func_800202c4()) {}
        for(i=0;i<4;i++) func_800154a4();
        func_800109c0();
        if(func_800108e0(22050,(u8)channels,(u8)enabled,(u8)control,0,memory)) {
        }
    } else {
        func_80010840(22050,(u8)channels,(u8)enabled,(u8)control,0,memory);
        D_8011EAA4=1;
    }
    for(instrument=D_8011F0A0; instrument<D_8011F0A0+4;instrument++) {
        func_8001536c(D_80152464,instrument->id,D_801525FC,D_80152690,D_801526D8);
    }
    func_8002043c(127,0,255);
    func_80020494(127,0,1,1);
    func_800A4CB8(control);
    D_8011EAA0=-1;
}
