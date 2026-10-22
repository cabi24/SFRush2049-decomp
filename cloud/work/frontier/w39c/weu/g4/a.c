typedef unsigned char u8;
typedef short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {
    u8 pad00[0x58];
    u8 *dl_base;           /* 0x58 */
    s16 unk5C;             /* 0x5C */
    u8 pad5E[0x7C - 0x5E];
    u32 buffer;            /* 0x7C */
} SegmentTableEntry;
extern s32 D_8015F738, D_80161380, D_80161398, D_801613A4;
extern f32 D_80154188;
extern u32 D_80000300;     /* osTvType */
extern s32 D_8002AFC0, D_8002AFC4;
extern u32 D_8002EBB0, D_80156CEC, D_80157240;
extern SegmentTableEntry D_80156BE0[];
extern u8 D_800586D0[], D_8006A090[];
extern u8 *D_8015B250, *D_8015B260;
extern u8 *D_801497C8;           /* 0x801497C8, display-list cursor */
extern u8 *D_801497F4;
s32 get_tv_offset(void);
void viewport_setup(s32 tv, s32 mode, s32 width, s32 height);
u32 audio_dma_sync(s32 arg0, s32 size);
u32 sound_play_menu(s32 arg0, s32 size);
void audio_loop_control(u32 buf, s32 arg1);
void func_800A5B3C(void);
void func_800A5488(void);
void particle_velocity_set(void);

void func_800EE8AC(void);

void world_effect_update(void)
{
    func_800EE8AC();
    D_801497F4 = D_801497C8;
    func_800A5B3C();
    func_800A5488();
    particle_velocity_set();
}
