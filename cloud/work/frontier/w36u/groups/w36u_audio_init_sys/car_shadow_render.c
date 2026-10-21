/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * car_shadow_render (0x800A5D34, 836 B) -- historical label; really the N64 AudioInitSys
 * (MAT3 audiosysps2.c AudioInitSys(maxVoices, maxSfx), GC 0x8009ED90: the GC body is the PS2 layer,
 * only the final AudioSfxInit(maxSfx) call is shared -- func_800A4CB8 = AudioSfxInit, here
 * AudioSfxInit(control)). N64 platform part reconstructed from the disassembly, building on the
 * frozen A142 reconstruction (cloud/work/tiny_A142/car_shadow_render.c, 72/209) -- credit A142.
 *
 * Calls: LoadWave = audio_frame_sync, LockWaveInstance = display_list_alloc,
 * _ValidateWaveInstance = func_80096288; GetWaveInstanceAddr (MAT3 waves.c, GC 0x8005B7B4:
 * `_ValidateWaveInstance(idx,0,0); return gWaveInstance[idx].ptr;`) is umerge-inlined three times, which
 * is what leaves the header pointer in v0 before each LoadWave call. gWaveInstance = D_80156D38
 * (20-byte records, ptr at +0xC). Static-library prototypes are the promoted ones (src/rom/lib_113e0.c,
 * lib_15a10.c, lib_207b0.c): func_80020274(void), func_800202C4 returns u8, func_80010840/func_800108E0
 * (u32 frequency, u8 voices, u8, u8, u16 count, u32 flags), func_8002043C(u8, u16, u8), func_80020494(u8, u16, u8, u8).
 *
 * D_00039B40 is the ROM offset of an asset (retail builds it with lui 0x4 / addiu -0x64C0, i.e. as a
 * symbol address, not as the integer 0x39B40 which IDO would build with lui/ori); it is passed to
 * func_8001536C as the sample `address`. Coordinator decision needed on the name of this absolute symbol.
 * GetWaveInstanceAddr is a non-static, non-kept definition (w17j: no deleted-static stub).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct { u8 id,other1,mode,other3; s32 word4,word8; void *hdr; s32 word16; } Resource20;
typedef struct { u16 other,id; } Instrument4;
extern u8 D_80151960;
extern s8 D_8011EAA4;
extern s32 D_8011EAA0;
extern u8 D_80152750[],D_8015276C[];
extern u8 D_80035260[],D_800350B0[],D_80033EA0[],D_8002F660[];
extern Resource20 D_80156D38[];
extern Instrument4 D_8011F0A0[];
extern void *D_80152464,*D_801526D8,*D_80152690;
extern s32 D_801525FC;
extern u8 D_00039B40[];
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
extern void func_80020274(void);
extern u8 func_800202c4(void);
extern s32 func_800154a4(void);
extern void func_800109c0(void);
extern s32 func_800108e0(u32,u8,u8,u8,u16,u32);
extern s32 func_80010840(u32,u8,u8,u8,u16,u32);
extern void func_8001536c(void *,u16,s32,void *,void *);
extern void func_8002043c(u8,u16,u8);
extern void func_80020494(u8,u16,u8,u8);
extern void func_800A4CB8(s32);

void *GetWaveInstanceAddr(s32 idx);

void car_shadow_render(s32 channels,s32 control,s32 enabled)
{
    u32 memory;
    s32 handle,i;
    s32 j;
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
        D_80152464=GetWaveInstanceAddr(handle);
        handle=audio_frame_sync(7,0,0,0,0);
        display_list_alloc(handle);
        D_801526D8=GetWaveInstanceAddr(handle);
        handle=audio_frame_sync(8,0,0,0,0);
        display_list_alloc(handle);
        D_80152690=GetWaveInstanceAddr(handle);
        D_801525FC=(s32)D_00039B40;
        func_80020598(&D_8011EAAC);
    }
    if(D_8011EAA4) {
        func_80020274();
        while(!func_800202c4()) {}
        for(i=0;i<4;i++) func_800154a4();
        func_800109c0();
        if(func_800108e0(22050,(u8)channels,(u8)enabled,(u8)control,0,memory)) {
        }
    } else {
        func_80010840(22050,(u8)channels,(u8)enabled,(u8)control,0,memory);
        D_8011EAA4=1;
    }
    for(j=0;j<4;j++) {
        func_8001536c(D_80152464,D_8011F0A0[j].id,D_801525FC,D_80152690,D_801526D8);
    }
    func_8002043c(127,0,255);
    func_80020494(127,0,1,1);
    func_800A4CB8(control);
    D_8011EAA0=-1;
}

void *GetWaveInstanceAddr(s32 idx)
{
    func_80096288(idx, 0, 0);
    return D_80156D38[idx].hdr;
}
