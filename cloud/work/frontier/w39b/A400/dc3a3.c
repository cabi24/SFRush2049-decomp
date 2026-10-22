/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w39b: func_8038A400 = updateWarningScreen@warningscr.c, straight port of the Dreamcast warningscr.c text (closest
 * to the N64 body: MAT3 keeps only the tail) with the N64 additions read from the retail words: 4 paks,
 * PS_GetBlocks(0x810), the gActiveEdges 0xC00 gPakScr toggle and the `& 3` start test. Statics as in DC. */
typedef signed char s8; typedef unsigned char u8;
typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct Blit Blit;
typedef struct MultiBlit MultiBlit;
typedef struct Controller { s8 attached; u8 rest[15]; } Controller;

extern s8 D_80116DB4;
extern void *D_8012E6E0;
extern Controller D_80156CF0[4];  /* gControllerData */
extern u32 D_8015694C;            /* gActiveEdges */
extern u8 D_803BA7EB;             /* gBlocksNeeded */
extern MultiBlit D_803B83A4[];    /* WarningBlit */

extern s8 func_800A1A3C(s32);     /* PS_PakInserted */
extern s8 func_800A35F8(s32);     /* PS_PakDamaged */
extern s8 func_80094FC4(s32);     /* PS_PakRumble */
extern u32 func_800A35BC(s32);    /* PS_GetFreeBlocks */
extern u32 func_800A3508(s32);    /* PS_GetBlocks */
extern u32 func_800A3518(s32);    /* PS_GetFreeFiles */
extern Blit *sound_control(s16, s16, MultiBlit *, s16); /* NewMultiBlit */
extern void sound_stop(Blit *);   /* RemoveBlit */
extern void player_state_set(s32, s32); /* SetControllerActive */
extern void player_mode_set(s32, s32);  /* SetControllerStickEdges */
extern void audio_distance_atten(u32);  /* PlayUDSound */
extern void resource_type_select(u32);  /* PlayStartABSound */
extern void func_800B5570(s32);   /* setGameState */

static s8 gPakScr = 0;            /* D_803B839C */
static u8 numPaksFound = 0;       /* D_803B83A0 (DC byte_8C0D19E1) */

void func_8038A400(void)
{
    static Blit *sBlit = 0;       /* D_803B83C8 */
    s32 numPaks;
    s32 i;

    if (!sBlit) {
        gPakScr = 0;
        D_80116DB4 = 0;
    }
    if (!D_8012E6E0) {
        i = 0;
        do {
            if (D_80156CF0[i].attached) break;
            i++;
        } while (i < 4);
        if (i < 4) {
            numPaks = 0;
            for (i = 0; i < 4; i++) {
                if (func_800A1A3C(i) && !func_800A35F8(i) && !func_80094FC4(i)) {
                    numPaks++;
                    if (func_800A35BC(i) >= func_800A3508(0x810) && func_800A3518(i) >= 1) goto done;
                }
            }
            numPaksFound = numPaks;
            if (numPaksFound > 0 || !D_80116DB4) {
                if (!sBlit) {
                    sBlit = sound_control(0, 0, D_803B83A4, 1);
                    player_state_set(-1, 1);
                    player_mode_set(-1, 1);
                }
                if (numPaksFound > 0) {
                    if (D_8015694C & 0xc00) {
                        audio_distance_atten(D_8015694C);
                        gPakScr = !gPakScr;
                    }
                } else {
                    gPakScr = 0;
                }
                if (!(3 & D_8015694C)) return;
                resource_type_select(D_8015694C);
            }
        }
    }
done:
    if (sBlit) {
        sound_stop(sBlit);
        sBlit = 0;
    }
    D_803BA7EB = func_800A3508(0x810);
    func_800B5570(gPakScr ? 0x4000000 : 4);
}
