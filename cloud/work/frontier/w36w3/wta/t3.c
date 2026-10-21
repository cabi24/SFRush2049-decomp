/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct { u8 pad[636]; s32 frameCount; } Sched;

extern OSMesgQueue D_8002ECF8;          /* gfx thread queue */
extern OSMesgQueue D_8002ECC0;
extern u8 D_80142D90[];
extern volatile s8 D_80035470, D_80035471, D_80035472;
extern volatile Sched D_8002E8E8;
extern volatile s32 D_8002EB98;
extern volatile u32 D_80114730;
extern s32 D_80151AE0;
extern volatile u16 D_801525F0;
extern s8 D_8011EAE0;
extern volatile s8 D_8015F72D;
extern s32 D_801161CC;
s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
void *osViGetCurrentFramebuffer(void);
void *osViGetNextFramebuffer(void);
void osViBlack(u8);
void func_800205E4(void);
void apply_display_mode(void);
void check_mpath_save(void);
void gfx_frame_begin(void);
void world_collision_response(void);

void wta_prenmi(void)
{
    func_800205E4();
    apply_display_mode();
    osViBlack(1);
    D_801525F0 = 0;
    check_mpath_save();
    D_8011EAE0 = 0;
    for (;;) {
    }
}

void wta_retrace(void)
{
    void *cur;

    cur = osViGetCurrentFramebuffer();
    if (osViGetNextFramebuffer() == cur && !D_80035471 && !D_80035470 && !D_80035472 &&
        D_8002E8E8.frameCount != D_8002EB98 && D_80114730 < 2) {
        D_80151AE0 = 0;
        gfx_frame_begin();
        D_80035470 = 1;
        osSendMesg(&D_8002ECC0, D_80142D90, 1);
    }
    D_80151AE0++;
}

void wta_done(void)
{
    D_80035472 = 0;
    world_collision_response();
    D_8015F72D ^= 1;
    D_80114730++;
}

void wta_dpdone(void)
{
    D_801161CC ^= 1;
    D_80114730--;
}

void world_trigger_activate(void)
{
    OSMesg msg = 0;
    /*@{c*/void *cur;/*@|@}*/

    osRecvMesg(&D_8002ECF8, &msg, 1);
    switch (*(s16 *)msg) {
    case 4:
        /*@{A*/func_800205E4();
        apply_display_mode();
        osViBlack(1);
        D_801525F0 = 0;
        check_mpath_save();
        D_8011EAE0 = 0;
        for (;;) {
        }/*@| wta_prenmi(); @}*/
    case 1:
        /*@{c*/cur = osViGetCurrentFramebuffer();
        if (osViGetNextFramebuffer() == cur && !D_80035471 && !D_80035470 && !D_80035472 &&
            D_8002E8E8.frameCount != D_8002EB98 && D_80114730 < 2) {
            D_80151AE0 = 0;
            gfx_frame_begin();
            D_80035470 = 1;
            osSendMesg(&D_8002ECC0, D_80142D90, 1);
        }
        D_80151AE0++;/*@| wta_retrace(); @}*/
        break;
    case 2750:
        /*@{D*/D_80035472 = 0;
        world_collision_response();
        D_8015F72D ^= 1;
        D_80114730++;/*@| wta_done(); @}*/
        break;
    case 2:
        /*@{E*/D_801161CC ^= 1;
        D_80114730--;/*@| wta_dpdone(); @}*/
        break;
    }
}
