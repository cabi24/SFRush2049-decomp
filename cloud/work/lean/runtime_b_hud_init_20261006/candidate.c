/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B HUD initialization, 0x803925D0..0x80392894. Native reconstruction. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Blit Blit;
typedef struct MultiBlit {
    const char *texname;
    s16 x, y, width, height, top, bottom, left, right;
    unsigned int depth, alpha;
    s32 (*callback)(Blit *);
    unsigned int id;
} MultiBlit;
typedef struct FiveParts FiveParts;
typedef struct HudEffect { s16 state, handle; u8 unknown04[0x44]; } HudEffect;
typedef struct HudEffectGroup { HudEffect effect[10]; } HudEffectGroup;
typedef struct HudStatus { s16 state, handle; u8 unknown04[0x30]; } HudStatus;
typedef struct HudHandle { s16 handle; u8 unknown02[0x32]; } HudHandle;
typedef struct Position { s32 x, y; } Position;
typedef struct Colors { u8 first[4], second[4]; } Colors;
extern Blit *D_80395ED4;
extern s32 D_8014A110;
extern s16 D_80151AD0, D_8014A108;
extern MultiBlit D_80393DE8[], D_80393B84[];
extern u8 D_80394A80[];
extern Position D_80393F50[4][4];
extern Colors D_80394250[4];
extern s8 D_80395ED0, D_80395E70, D_80395E88, D_80395EA0, D_80395EB8;
extern HudEffectGroup D_803950C0[4];
extern HudStatus D_80395C00[4], D_80395CD0[4];
extern HudHandle D_80395DA0[4];
extern Blit *sound_control(s16, s16, const MultiBlit *, s16);
extern s32 object_create(s32);
extern s32 object_manager_update(u8 *, s16);
extern s16 object_bytes_sum_global(void);
extern FiveParts *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);
extern void tournament_unlock_check(FiveParts *, const u8 *, const u8 *);

void func_803925D0(void)
{
    s32 i, j, player;
    s32 width, height;
    FiveParts *panel;
    if (D_80395ED4 == 0) {
        if (D_8014A110 == 6) {
            D_80395ED4 = sound_control(0, 0, D_80393DE8, 10);
        } else if (D_8014A110 == 4) {
            if (D_80151AD0 == 1) {
                object_create(10);
            } else if (D_80151AD0 == 2) {
                object_create(11);
            } else {
                object_create(12);
            }
            width = object_manager_update(D_80394A80, -1) + 8;
            height = object_bytes_sum_global() + 4;
            for (i = 0; i < D_8014A108; i++) {
                panel = ambient_sound_set(
                    D_80393F50[D_80151AD0 - 1][i].x - width / 2,
                    D_80393F50[D_80151AD0 - 1][i].y - height / 2,
                    D_80393F50[D_80151AD0 - 1][i].x + width / 2,
                    D_80393F50[D_80151AD0 - 1][i].y + height / 2,
                    128, 0, i << 4, 1);
                tournament_unlock_check(panel, D_80394250[i].first, D_80394250[i].second);
            }
            D_80395ED4 = sound_control(0, 0, D_80393B84, 17);
        }
        D_80395ED0 = 0;
        D_80395E70 = 0;
        D_80395E88 = 0;
        D_80395EA0 = 0;
        D_80395EB8 = 0;
        for (player = 0; player < 4; player++) {
            D_80395C00[player].state = 8;
            D_80395C00[player].handle = -1;
            D_80395CD0[player].state = 0;
            D_80395CD0[player].handle = -1;
            D_80395DA0[player].handle = -1;
            for (j = 0; j < 10; j++) {
                D_803950C0[player].effect[j].state = 11;
                D_803950C0[player].effect[j].handle = -1;
            }
        }
    }
}
