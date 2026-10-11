/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w14x draft: func_800CC50C written from the retail disassembly (tdis.py), group with real func_800C7200. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
#define NULL ((void *)0)
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))
#define TQ(t) (*(u8 **)FLD((t), 0x28, void *))

extern f32 D_80111754[];
extern s8 D_8014978C;
extern f32 D_8002AFB4;
extern s32 D_80152770;
extern s32 D_8013F1F8;
extern s32 D_8013F390;

s32 osRecvMesg(void *mq, void *msg, s32 flags);
s32 osJamMesg(void *mq, void *msg, s32 flags);
void *memset(void *dst, s32 c, u32 n);
void *memcpy(void *dst, void *src, u32 n);
void func_800A47C0(void *a, void *b, s32 n);
s32 format_string_parse(void *p, s32 n);
void *sound_play_menu(s32 a, s32 b);
s32 car_angular_velocity_clamp(void *a, s32 b, void *c, s32 d, s32 e, s32 f, s32 g);
void audio_reverb_update(u32 unused, void *addr, s32 tag);
void **audio_task_complete(void *a, s32 b);
s32 audio_buffer_sync(void **p);
void **func_800C7200(void);
void **func_800CC50C(void *arg0, s8 *arg1);

void **func_800C7200(void) {
    void **var_a0;
    s32 var_v1;

    var_v1 = 0; var_a0 = (void **)&D_8013F1F8;
loop_1:
    if (FLD(var_a0, 0, void *) == NULL) {
        FLD(var_a0, 0, void *) = (void *) ((var_v1 * 0x2C) + (u8 *) &D_8013F390);
        return var_a0;
    }
    if (FLD(var_a0, 4, void *) == NULL) {
        FLD(var_a0, 4, void *) = (void *) ((var_v1 * 0x2C) + 0x2C + (u8 *) &D_8013F390);
        return (void **)((u8 *)var_a0 + 4);
    }
    if (FLD(var_a0, 8, void *) == NULL) {
        FLD(var_a0, 8, void *) = (void *) ((var_v1 * 0x2C) + 0x58 + (u8 *) &D_8013F390);
        return (void **)((u8 *)var_a0 + 8);
    }
    if (FLD(var_a0, 0xC, void *) == NULL) {
        FLD(var_a0, 0xC, void *) = (void *) ((var_v1 * 0x2C) + 0x84 + (u8 *) &D_8013F390);
        return (void **)((u8 *)var_a0 + 0xC);
    }
    var_v1 += 4;
    var_a0 = (void **)((u8 *)var_a0 + 0x10);
    if (var_v1 == 0x40) {
        goto exhausted;
    }
    goto loop_1;
exhausted:;
}

void **func_800CC50C(void *arg0, s8 *arg1) {
    void **p1;
    void **slot;
    s32 sp50;
    u8 *base;
    void *sp54;
    s32 n;
    u32 unused;
    f32 f;
    s32 sp44;
    u8 *task;
    s32 off;


     
    base = FLD(arg0, 0x4C, u8 *);
     off = (s32)(D_8002AFB4 * (f32)((f64)D_80111754[D_8014978C] + 10.0)); 
    func_800A47C0( FLD(arg0, 0x54, s32) + base , base + off, FLD(arg0, 0x54, s32));
    func_800A47C0(FLD(arg0, 0x4C, u8 *) + FLD(arg0, 0x54, s32) * 2, FLD(arg0, 0x4C, u8 *) + off * 2, FLD(arg0, 0x54, s32));
    off = FLD(arg0, 0x54, s32);
    n = off * 3;
    sp44 = format_string_parse(base, n);
    
     if (base) {} 
    sp54 = sound_play_menu(0, n);
    
    sp50 = car_angular_velocity_clamp(base, n, sp54, n, 9, 10, 4);
    FLD(arg0, 0x24, s16) = 0;
    
    osRecvMesg(&D_80152770, NULL, 1);
    audio_reverb_update( 0 , FLD(arg0, 0x4C, u8 *), 0);
    osJamMesg(&D_80152770, NULL, 0);
    FLD(arg0, 0x4C, void *) = NULL;
    slot = func_800C7200();
    
    task = (u8 *)*slot;
    memset(task, 0, 44);
    p1 = audio_task_complete(NULL,  0x58 + sp50 );
    FLD(task, 0x28, void **) = p1;
    memcpy(*p1, arg0, 88);
    FLD(TQ(task), 4, s8) = 17;
    FLD(TQ(task), 0x4C, s32) = 0;
    FLD(TQ(task), 0x50, s32) = 0;
    FLD(TQ(task), 0x54, s32) = 0;
    
    FLD(TQ(task), 0x3C, s32) = sp50;
    FLD(TQ(task), 0x40, s32) = n;
    base = TQ(task);
    FLD(base, 0, s32) = format_string_parse(TQ(task) + 4, 0x40);
    FLD(TQ(task), 0x44, s32) = format_string_parse(sp54, sp50);
    FLD(TQ(task), 0x48, s32) = sp44;
    memcpy(TQ(task) + 0x58, sp54, sp50);
    osRecvMesg(&D_80152770, NULL, 1);
    audio_reverb_update(unused, sp54, 0);
    osJamMesg(&D_80152770, NULL, 0);
    FLD(task, 8, s8) = FLD(TQ(task), 6, s8);
    FLD(task, 9, u8) = FLD(TQ(task), 7, u8);
    memcpy(task + 10, TQ(task) + 22, 13);
    FLD(task, 0x18, s32) = FLD(TQ(task), 0x28, s32);
    FLD(task, 0x1C, s32) = FLD(TQ(task), 0x2C, s32);
    FLD(task, 0x20, f32) = FLD(TQ(task), 0x38, f32);
    *arg1 = (s8)((audio_buffer_sync(p1) +  255 ) >> 8);
    return slot;
}

