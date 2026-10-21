/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8;
typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct Blit Blit;
typedef struct MultiBlit MultiBlit;
typedef struct Controller { s8 attached; u8 rest[15]; } Controller;

extern s8 D_80116DB4;
extern void *D_8012E6E0;
extern Controller D_80156CF0[4];  /* gControllerData */
extern u32 D_8015694C;            /* gActiveEdges */
extern s8 D_803B839C;             /* gPakScr */
extern u8 D_803B83A0;             /* number of usable paks */
extern MultiBlit D_803B83A4[];    /* mblit */
extern Blit *D_803B83C8;          /* sBlit */
extern u8 D_803BA7EB;             /* gBlocksNeeded */

extern s8 func_800A1A3C(s32);     /* PS_PakInserted */
extern s8 func_800A35F8(s32);     /* PS_PakDamaged? */
extern s8 func_80094FC4(s32);     /* pak busy */
extern u32 func_800A35BC(s32);    /* PS_GetFreeBlocks */
extern s32 func_800A3508(s32);    /* PS_GetBlocks */
extern s32 func_800A3518(s32);    /* PS_GetFreeFiles */
extern Blit *sound_control(s16, s16, MultiBlit *, s16); /* NewMultiBlit */
extern void sound_stop(Blit *);   /* RemoveBlit */
extern void player_state_set(s32, s32); /* SetControllerActive */
extern void player_mode_set(s32, s32);  /* SetControllerStickEdges */
extern void audio_distance_atten(u32);  /* PlayUDSound */
extern void resource_type_select(u32);  /* PlayStartABSound */
extern void func_800B5570(s32);   /* setGameState */

static s32 anyAttached(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (D_80156CF0[i].attached)
            break;
    }
    return i < 4;
}

void func_8038A400(void)
{
    s32 pak;
    u32 freeBlocks;
    s32 numPaks;

    if (!D_803B83C8) {
        D_803B839C = 0;
        D_80116DB4 = 0;
    }
    if (!D_8012E6E0) {
        if (anyAttached()) {
            numPaks = 0;
            for (pak = 0; pak < 4; pak++) {
                if (func_800A1A3C(pak) && !func_800A35F8(pak) && !func_80094FC4(pak)) {
                    numPaks++;
                    freeBlocks = func_800A35BC(pak);
                    if (freeBlocks >= func_800A3508(2064) && func_800A3518(pak))
                        goto done;
                }
            }
            D_803B83A0 = numPaks;
            if (D_803B83A0 > 0 || !D_80116DB4) {
                if (!D_803B83C8) {
                    D_803B83C8 = sound_control(0, 0, D_803B83A4, 1);
                    player_state_set(-1, 1);
                    player_mode_set(-1, 1);
                }
                if (D_803B83A0 > 0) {
                    if (D_8015694C & 0xc00) {
                        audio_distance_atten(D_8015694C);
                        freeBlocks = D_803B839C;
                        D_803B839C = !freeBlocks;
                    }
                } else {
                    D_803B839C = 0;
                }
                if (!(D_8015694C & 3))
                    return;
                resource_type_select(D_8015694C);
            }
        }
    }
done:
    {
        Blit *b = D_803B83C8;
        if (b) {
            sound_stop(b);
            D_803B83C8 = 0;
        }
    }
    D_803BA7EB = func_800A3508(2064);
    freeBlocks = D_803B839C ? 0x4000000 : 4;
    func_800B5570(freeBlocks);
}
