/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w36g superseding revision (wave 36): func_8039A4D4 (updateButtons@circuitscr.c, formerly context) and
 * func_803A5948 (animText@circuitscr.c with animButtonText and getFinishPlaceStr inlined) plus its two
 * deleted-inline stubs func_803A5938/func_803A5940 are new members; see their headers below. All thirteen
 * old members and their text are unchanged. */
/* Image-A A5658 strict match: 736 new bytes. This same-name group preserves
 * all eleven installed text_helpers members and pending PR333 BB80 (712 B).
 * BB80 is preserved-only credit here, not another new match.
 * BB80 computes the configuration panel width/height from localized labels
 * and up to two player names/statistics. N64 reconstruction; arcade ancestor
 * is not established. Its private ABI saves ra, and clobbers s0..s8.
 *
 * BB80 SHAPING: the two genuine player/text helpers use player-first formal
 * order. Both helper bodies remain byte-identical; every real in-group call
 * is updated. This recovers native private-argument evaluation ordering.
 * Eight disclosed unused s32 homes are the exact frame residual: the same
 * live 80-byte text array moves 48 -> 80; frame128 + 8*4 = native160. Their
 * original names/purposes are unknown. No live capacity is changed.
 *
 * Inherited C140 shaping: two meaningful operand carriers account for its
 * native frame136; the one-line animation macro preserves a stock as1 source-
 * line scheduling tie. Inherited volatile clock declaration and pointer/record
 * hypotheses are unchanged; asynchronous clock behavior is not established.
 * B560 retains separate same-value 55.2 literal spellings and owned float data.
 * No false return types, fabricated callees, forced registers, compiler/object
 * patches, padding arrays, or historical-source spelling claims are used.
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad0[4]; u8 u4; u8 pad5[3]; s8 b8; s8 b9; } Cfg;
extern Cfg *D_803BAD90;
extern s8 D_80154640;
extern s8 D_803B3424;
extern s8 D_803B3428;
extern u8 D_80150E88[];
extern u8 D_80150DDB[];
extern u8 D_80150F7C[];
extern s8 D_80156994;

void func_8039B120(void)
{
    D_803B3424 = !(D_803BAD90->b9 >= D_80154640) && D_803BAD90->u4 != 0;
    D_803B3428 = D_803BAD90->u4;
    if (D_803BAD90->b8 > 0) {
        D_80150E88[1] = 1;
        D_80150DDB[1] = 1;
        D_80150F7C[1] = 1;
    }
    if (D_803BAD90->b8 >= 2 && D_80156994 != 0) {
        D_80150E88[2] = 1;
        D_80150DDB[2] = 1;
        D_80150F7C[2] = 1;
    }
    if (D_803BAD90->b8 == 3) {
        D_80150E88[3] = 1;
        D_80150F7C[3] = 1;
    }
    if (D_80156994 == 0 && D_803BAD90->b8 == 2) {
        D_803B3424 = 0;
    }
}

extern u32 D_80149784;
extern s32 D_8015694C;
extern s16 D_803B9A80, D_803B9A82, D_803B9A84;
extern u8 D_803BAD98[20];
extern u8 *D_80116FE4;
extern s8 D_803B341C, D_803B3420, D_803B3414;
extern s32 D_803B33C4;
extern u32 entity_flags_apply(u32,u32,u32,u8);
extern s32 stat_race_start(u8 *);
extern void resource_type_select_simple(s32);

void func_8039B214(void)
{
    s32 result;
    if (D_80149784 & 0x400) {
        entity_flags_apply(0x28, 0, 1, 0);
        if (D_803B9A80 == -2) D_803B9A80 = 21;
        else if (D_803B9A80 < 8) D_803B9A80 += 24;
        else if (D_803B9A80 < 12) D_803B9A80 = -2;
        else D_803B9A80 -= 12;
    } else if (D_80149784 & 0x800) {
        entity_flags_apply(0x27, 0, 1, 0);
        if (D_803B9A80 == -2) D_803B9A80 = 9;
        else if (D_803B9A80 < 20) D_803B9A80 += 12;
        else if (D_803B9A80 < 24) D_803B9A80 = -2;
        else D_803B9A80 -= 24;
    } else if (D_80149784 & 0x1000) {
        entity_flags_apply(0x29, 0, 1, 0);
        if (D_803B9A80 == -2) D_803B9A80 = 31;
        else if (D_803B9A80 == 24) D_803B9A80 = -2;
        else if (D_803B9A80 % 12 == 0) D_803B9A80 += 11;
        else D_803B9A80--;
    } else if (D_80149784 & 0x2000) {
        entity_flags_apply(0x2C, 0, 1, 0);
        if (D_803B9A80 == -2) D_803B9A80 = 24;
        else if (D_803B9A80 == 31) D_803B9A80 = -2;
        else if (D_803B9A80 % 12 == 11) D_803B9A80 -= 11;
        else D_803B9A80++;
    }
    if (D_8015694C & 2) {
        entity_flags_apply(0x25, 0, 1, 0);
        if (D_803B9A80 == -2) {
            if (D_803B9A82 > 0) {
                D_803B9A82--;
                D_803BAD98[D_803B9A82] = '.';
                D_803B341C = 0;
                D_803B3420 = 0;
            }
        } else if (D_803B9A82 < 20) {
            D_803BAD98[D_803B9A82] = D_80116FE4[D_803B9A80];
            D_803B9A82++;
            if (D_803B9A82 == 20) {
                result = stat_race_start(D_803BAD98);
                if (result == 0) D_803B341C = 1;
                else if (result == -1) D_803B3420 = 1;
                else {
                    D_803B3414 = 0;
                    func_8039B120();
                    D_803B33C4 = 1;
                    D_803B9A84 = D_803BAD90->b9;
                }
            }
        }
    } else if (D_8015694C & 5) {
        resource_type_select_simple(D_8015694C);
        D_803B3414 = 0;
        D_803B33C4 = 1;
    }
}

/* The main-blob wrapper forwards the allocator's s32 result unchanged.
 * Its genuine returning body is supplied separately in wrapper_return.c;
 * it must not be inlined into this separately compiled runtime image.
 * Production definitions and locks are unchanged. */
extern s32 sign_extend_call(u32 value, u32 data, s32 index, u32 flags);

typedef struct Item { u8 other[16]; u8 player; } Item;
typedef struct Selection { Item *item; } Selection;
typedef struct Resource { u8 *bytes; } Resource;
typedef struct MenuOwner MenuOwner;
typedef struct OwnerBody {
    MenuOwner *next, *previous;
    Selection *selection;
    u8 other[32];
    Resource *resource;
} OwnerBody;
struct MenuOwner { OwnerBody *body; };
typedef struct Player { u8 other[72]; MenuOwner *owner; } Player;
typedef struct Object { u8 other[32]; f32 scale; u8 tail[32]; } Object;
typedef struct Blit Blit;
typedef struct FiveParts FiveParts;
typedef struct Texture Texture;
typedef struct MultiBlit {
    char *name;
    s16 x,y,width,height,top,bottom,left,right;
    u32 z,alpha;
    s32 (*callback)(Blit *);
    u32 descriptor;
} MultiBlit;
extern s16 D_8014A108;
extern s32 D_8014A110;
extern Player D_8014A118[];
extern s32 D_803BAD38;
extern s8 D_80154451, D_803B342C;
extern u8 D_80140BDC;
extern char *D_803B3430[], *D_803B343C[];
extern f32 D_803B9A50[12], D_8011418C[12];
extern s32 D_803B3448, D_803B344C;
extern char D_803B8750[];
extern f32 D_803B95B4;
extern Object D_8012E6F0[];
extern s32 D_803B3450[];
extern s32 D_803BACF8;
extern s8 D_803B3418, D_803BADC4, D_803B340C;
extern FiveParts *D_803BACF0;
extern MultiBlit D_803B33E4[];
extern Blit *D_803B3408;
extern void object_byte71_set_sync(u8 ***,s32);
extern void net_session_update(void);
extern s32 string_copy_format(char *,s8,s8,s8);
extern Texture *func_800B24EC(char *,s16 *,s8,s8,s32);
extern void func_8008D870(s16,s32,s32);
extern void func_8008E06C(s16,s32 *);
extern void func_80092BC8(s16,s32 *);
extern void model_transform_setup(s32,s32,u32);
extern void math_utility(void *,void *);
extern void particle_velocity_set(void);
extern void particle_lifetime_set(void);
extern void func_8039BB80(void);
extern void func_8039B560(void);
extern FiveParts *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);

void func_8039BE48(void)
{
    s32 i;
    s32 selected;
    s32 model;
    s16 texture_index;
    for (i = 0; i < D_8014A108; i++) {
        object_byte71_set_sync((u8 ***)D_8014A118[i].owner, D_8014A110);
    }
    D_803BAD38 = 0;
    if (D_803BAD90->u4 && D_803BAD90->b9 == 0)
        D_803BAD90->u4 = 0;
    D_803B9A84 = D_803BAD90->b9;
    if (D_803BAD90->u4) {
        net_session_update();
        if (D_803BAD90->b9 >= D_80154640) {
            selected = D_80154451;
            D_803B342C = 1;
            if (selected < 3) {
                model = string_copy_format(D_803B3430[selected], 0, D_80140BDC - 1, 1);
                D_803B3448 = sign_extend_call(model, (u32)D_803B9A50, -1, 0);
                model = string_copy_format(D_803B343C[selected], 0, D_80140BDC - 1, 1);
                D_803B344C = sign_extend_call(model, (u32)D_8011418C, D_803B3448, 0x2C6084);
                func_8008D870((s16)D_803B344C,
                    (s32)func_800B24EC(D_803B8750, &texture_index, 0, D_80140BDC - 1, 1), -1);
                D_8012E6F0[D_803B344C].scale = D_803B95B4;
                func_8008E06C((s16)D_803B344C, D_803B3450);
                func_80092BC8((s16)D_803B344C, D_803B3450);
                model_transform_setup(D_803B3448, 0, 15);
                math_utility(D_8011418C, D_803B9A50);
                D_803B9A50[9] = 0.0f;
                D_803B9A50[10] = -13.0f;
                D_803B9A50[11] = 65.0f;
                particle_velocity_set();
            }
        }
    }
    func_8039B120();
    D_803BACF8 = 0;
    D_803B3414 = 0;
    D_803B3418 = 0;
    D_803BADC4 = 0;
    func_8039BB80();
    particle_lifetime_set();
    func_8039B560();
    D_803BACF0 = ambient_sound_set(-20, 0, -10, 10, 192, 0, 0, 0);
    D_803B3408 = sound_control(0, 0, D_803B33E4, 1);
    D_803B340C = 1;
}

extern u8 D_801543D4;
extern u32 D_801174BC, state_word_a;
extern s32 D_803B3410;
/* Existing same-symbol contract hypothesis; see pinned README references.
 * Native reloads support this access model, not asynchronous-writer claims. */
extern volatile f32 D_8002EB94;
extern f32 D_803B95B8;
extern s8 D_80157244;
extern u8 D_803B8738[];
extern void func_800B5570(s32);
extern void func_80090E9C(f32, f32 (*)[3]);
extern void func_80090254(s16);
extern u8 *func_800BE6A4(u8 *, u8 *);
extern void func_800FDF88(s32);
extern void audio_distance_atten(s32);
extern void resource_type_select(s32);
extern void func_800B5F88(s32);
extern s32 func_800F7564(s32);
extern s8 func_800A361C(s32);
extern void func_8039B00C(void);
extern void func_8039A4D4(void);

s32 func_8039A448(s32 mode)
{
    return (mode != 3 || D_8014A118[D_801543D4].owner->body->selection == 0)
        && (!(mode == 0) || D_803B3424 != 0)
        && (!(mode == 1) || D_803B3428 != 0);
}

/* Keep invocation and else-if on one line: measured as1 scheduling tie. */
#define C140_ANIMATION_ARM \
{ \
        matrix = (f32 (*)[3])D_803B9A50; \
        angle = -90.0f * D_8002EB94 * D_803B95B8; \
        func_80090E9C(angle, matrix); \
        if (D_8015694C & 7) { \
            if (D_803B3448 != -1) { \
                func_80090254((s16)D_803B3448); \
                D_803B3448 = -1; \
            } \
            if (D_803B344C != -1) { \
                func_80090254((s16)D_803B344C); \
                D_803B344C = -1; \
            } \
            resource_type_select_simple(D_8015694C); \
            D_803B342C = 0; \
            D_803B33C4 = 1; \
        } \
    }

void func_8039C140(void)
{
    s32 i;
    f32 (*matrix)[3];
    f32 angle;
    D_803BAD90 = (Cfg *)(D_8014A118[D_801543D4].owner->body->resource->bytes + 0x6F4);
    if (D_801174BC != 1) {
        if (D_801174BC != state_word_a) {
            func_800B5570(0x80);
            return;
        }
        D_801174BC = 1;
    }
    if (!D_803B340C)
        func_8039BE48();
    while (!func_8039A448(D_803BACF8)) {
        D_803BACF8++;
        if (D_803BACF8 >= 4)
            D_803BACF8 = 0;
    }
    if (D_803B342C) C140_ANIMATION_ARM else if (D_803B3414) {
        func_8039B214();
    } else if (D_8015694C & 1) {
        entity_flags_apply(0x2E, 0, 1, 0);
        if (D_803B3418) {
            D_803B3418 = 0;
            D_803BADC4 = 0;
            D_803B33C4 = 1;
        } else if (!func_8039A448(0)) {
            D_803BACF8 = 2;
            goto confirm_option;
        } else {
            net_session_update();
            D_803B3410 = 1;
        }
    } else if (D_8015694C & 2) {
        entity_flags_apply(0x25, 0, 1, 0);
 confirm_option:
        switch (D_803BACF8) {
        case 0:
            net_session_update();
            D_803B3410 = 1;
            break;
        case 1:
            break;
        case 3:
            D_803B3414 = 1;
            D_803B9A82 = 0;
            D_803B9A80 = D_803B9A82;
            func_800BE6A4(D_803BAD98, D_803B8738);
            D_803B341C = 0;
            D_803B3420 = 0;
            break;
        case 2:
            if (D_803BADC4 || !D_803B3424) {
                func_800FDF88(D_803BAD38);
                net_session_update();
                D_803B3410 = 1;
            } else if (D_803B3418) {
                D_803B3418 = 0;
                D_803BADC4 = 0;
                D_803B33C4 = 1;
            } else {
                D_803BADC4 = 0;
                D_803B3418 = 1;
            }
            break;
        }
    } else if (D_8015694C & 4) {
        entity_flags_apply(0x26, 0, 1, 0);
        if (D_803B3418) {
            D_803B3418 = 0;
            D_803BADC4 = 0;
            D_803B33C4 = 1;
        } else {
            D_803B3410 = 2;
        }
    } else if (D_8015694C & 0x400) {
        if (!D_803B3418) {
            audio_distance_atten(D_8015694C);
            do {
                D_803BACF8--;
                if (D_803BACF8 < 0)
                    D_803BACF8 = 3;
            } while (!func_8039A448(D_803BACF8));
        }
    } else if (D_8015694C & 0x800) {
        if (!D_803B3418) {
            audio_distance_atten(D_8015694C);
            do {
                D_803BACF8++;
                if (D_803BACF8 >= 4)
                    D_803BACF8 = 0;
            } while (!func_8039A448(D_803BACF8));
        }
    } else if (D_8015694C & 0x3000) {
        if (D_803B3418) {
            func_800B5F88(D_8015694C);
            D_803BADC4 = !D_803BADC4;
        } else if (D_803BACF8 == 2) {
            func_800B5F88(D_8015694C);
            do {
                if (D_8015694C & 0x1000)
                    D_803BAD38--;
                else if (D_8015694C & 0x2000)
                    D_803BAD38++;
                if (D_803BAD38 < 0)
                    D_803BAD38 = 3;
                if (D_803BAD38 >= 4)
                    D_803BAD38 = 0;
            } while (!func_800F7564(D_803BAD38));
        } else if (D_803BACF8 == 1) {
            func_800B5F88(D_8015694C);
            if (D_8015694C & 0x1000)
                D_803B9A84--;
            else if (D_8015694C & 0x2000)
                D_803B9A84++;
            if (D_803B9A84 < 0)
                D_803B9A84 = D_803BAD90->b9;
            if (D_803B9A84 > D_803BAD90->b9)
                D_803B9A84 = 0;
        }
    }
    for (i = 0; i < D_8014A108; i++) {
        if (D_8014A118[i].owner->body->selection) {
            if (func_800A361C(D_8014A118[i].owner->body->selection->item->player)) {
                func_8039B00C();
                func_800B5570(0x20);
                return;
            }
        }
    }
    if (D_80157244) {
        func_8039B00C();
        func_800B5570(4);
    } else if (D_803B3410) {
        func_8039B00C();
        if (D_803B3410 == 1) {
            switch (D_803BACF8) {
            case 0: func_800B5570(0x80); break;
            case 2: func_800B5570(0x80); break;
            }
        } else if (D_803B3410 == 2) {
            func_800B5570(0x40);
        }
        D_803B3410 = 0;
    }
    func_8039A4D4();
}

#undef C140_ANIMATION_ARM

typedef struct MenuObjectSlot {
    s32 other,handle;
    f32 angle;
    f32 matrix[3][3];
    f32 position[3];
    u8 alpha,unaccessed[3];
} MenuObjectSlot;
extern MenuObjectSlot D_803B30C0[12];
extern s32 D_803B33C0;
extern void sound_stop(Blit *);
extern void sound_handles_array_clear(s8 *);
extern void ambient_sounds_clear(void);
void func_8039B00C(void)
{
    s32 i;
    for (i = 0; i < 12; i++) {
        if (D_803B30C0[i].handle != -1) {
            if (state_word_a & 0x7C03FFFE)
                func_80090254((s16)D_803B30C0[i].handle);
            D_803B30C0[i].handle = -1;
        }
    }
    D_803B33C0 = 0;
    if (D_803B3408) {
        sound_stop(D_803B3408);
        D_803B3408 = 0;
    }
    if (D_803BACF0) {
        sound_handles_array_clear((s8 *)D_803BACF0);
        D_803BACF0 = 0;
    }
    if (D_803B3448 != -1) {
        func_80090254((s16)D_803B3448);
        D_803B3448 = -1;
    }
    if (D_803B344C != -1) {
        func_80090254((s16)D_803B344C);
        D_803B344C = -1;
    }
    ambient_sounds_clear();
    D_803B340C = 0;
}

typedef union Color {u32 word;u8 channel[4];} Color;
extern Color D_803B345C;
extern f32 D_803B33C8,D_803B33CC,D_803B33D0,D_803B33D4,D_803B33D8,D_803B33DC,D_803B33E0;
extern f32 D_803B957C,D_803B9580,D_803B9584,D_803B9588,D_803B958C,D_803B9590;
extern f32 D_803BA228,D_803BA22C;
extern char D_803B87C8[],D_803B87D8[];
extern u8 D_80140BDC;
extern void *memcpy(void *,const void *,u32);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
extern void model_transform_setup(s32,s32,u32);
extern void model_data_load(s32,s32,u32);
extern void func_8008B32C(f32 [3][3],f32 [3][3],f32);
extern void func_800B5898(f32,f32 [][3]);
extern void func_800B5940(f32,f32 [][3]);
extern Texture *func_800B24EC(char *,s16 *,s8,s8,s32);
extern void func_8008D870(s16,s32,s32);
extern void func_8008E06C(s16,s32 *);

/* w36g: func_8039A4D4 = updateButtons@circuitscr.c (MAT3 GC 0x8006A730, 0x89C B): per-frame scroll, fade and
 * texture of the four circuit-menu buttons, their right-hand value buttons (4, 5) and the cursor (7, 9). Direct
 * typed port of reference/repos/mat3 decomp/src/GameSource/src/circuitscr.c updateButtons; called only by
 * func_8039C140 (updateCircuitScreen) as its last statement, so it keeps the group's private ABI (saves only ra).
 * N64 names: buttonInfo D_803B30C0, buttonsInited D_803B33C0, buttonsJustInited D_803B33C4, gShowTrophy
 * D_803B342C, gEnterCircuitCode D_803B3414, gConfirmNewCircuit D_803B3418, gCircuitMenuSelect D_803BACF8,
 * gButtonScale D_803B33C8, gButtonSpace D_803B33CC, buttonX D_803B33D0, maxY D_803B33D4, minY D_803B33D8,
 * buttonZ D_803B33DC, cursorZ D_803B33E0, gFrameSeconds D_8002EB94 (volatile), unitmatrix D_8011418C,
 * gNumModels D_80140BDC, buttonColor initialiser D_803B345C, "BUTTON_SELECT"/"BUTTON" D_803B87C8/D_803B87D8;
 * MBOX_ShowObject model_transform_setup, MBOX_HideObject model_data_load, ScaleUV func_8008B32C, WPitchUV
 * func_800B5898, WRollUV func_800B5940, MBOX_FindTexture_WORK func_800B24EC, ZOID_SetTexture func_8008D870,
 * ZOID_SetColor func_8008E06C, menuItemValid func_8039A448.
 * N64 differences read from the retail words: unselected buttons settle to 0.0f (GC 0.08726646f); alpha ramps
 * linearly over one button pitch (GC fixed 80); single-precision speed math (fabsf intrinsic, 60.0f/10.0f).
 * cursorSpeed/scrollSpeed are the function-local statics (retail .bss 0x803BA228/0x803BA22C).
 * Shaping (the carscr sibling's measured idioms, w35a func_8039D6A4): fabsf on the right of the speed tests;
 * the second 255 of each alpha ramp is the int 255 (keeps 255.0f rematerialised, 60.0f hoisted into $f30);
 * gNumModels read through a volatile view (the group's shared declaration stays non-volatile for B560/BE48).
 * FRAME RESIDUAL (disclosed): thirteen unused s32 homes declared first. Retail frame 152 with the locals at
 * sp+60..100 (buttonColor +60, texIdx +66, clearScrollSpeed +75, selectedPos +76, prevPos +84) and 52 bytes of
 * homes above them that no instruction touches; without the homes ugen gives frame 96 (locals +56..96); with
 * them 152 and every slot as retail. Their original names/purpose are unknown (no MAT3/DWARF counterpart; an
 * inlined helper's locals land below the caller's, so it is not an inline). No padding array is used.
 * Own data: six float literals, the two .bss statics. */
void func_8039A4D4(void)
{
    static f32 cursorSpeed;
    static f32 scrollSpeed;
    s32 frame_home0, frame_home1, frame_home2, frame_home3, frame_home4, frame_home5, frame_home6;
    s32 frame_home7, frame_home8, frame_home9, frame_home10, frame_home11, frame_home12;
    s32 buttonIdx;
    f32 prevPos[3];
    f32 buttonY;
    f32 selectedPos = 0.0f;
    s8 clearScrollSpeed = 1;
    Texture *tex;
    s16 texIdx;
    Color buttonColor = D_803B345C;

    if (!D_803B33C0)
        return;
    if (D_803B342C == 1) {
        for (buttonIdx = 0; buttonIdx < 12; buttonIdx++) {
            if (buttonIdx == 6 || buttonIdx == 8 || buttonIdx == 10)
                model_transform_setup(D_803B30C0[buttonIdx].handle, 0, 15);
            else {
                D_803B30C0[buttonIdx].alpha = 0;
                model_data_load(D_803B30C0[buttonIdx].handle, 0, 15);
            }
        }
        return;
    }
    for (buttonIdx = 0; buttonIdx < 12; buttonIdx++)
        model_transform_setup(D_803B30C0[buttonIdx].handle, 0, 15);
    buttonY = D_803B33D4;
    for (buttonIdx = 0; buttonIdx < 4; buttonIdx++) {
        if (D_803B3414 || D_803B3418) {
            model_data_load(D_803B30C0[buttonIdx].handle, 0, 15);
            if (buttonIdx == 1)
                model_data_load(D_803B30C0[4].handle, 0, 15);
            else if (buttonIdx == 2)
                model_data_load(D_803B30C0[5].handle, 0, 15);
        } else {
            if (buttonIdx == D_803BACF8) {
                selectedPos = buttonY;
                if (D_803B30C0[buttonIdx].angle < 3.1415927f)
                    D_803B30C0[buttonIdx].angle += 12.566371f * D_8002EB94;
                if (D_803B30C0[buttonIdx].angle > 3.1415927f)
                    D_803B30C0[buttonIdx].angle = 3.1415927f;
                if (D_803B33C4)
                    D_803B30C0[buttonIdx].angle = 3.1415927f;
            } else {
                if (D_803B30C0[buttonIdx].angle > 0.0f)
                    D_803B30C0[buttonIdx].angle -= 12.566371f * D_8002EB94;
                if (D_803B30C0[buttonIdx].angle < 0.0f)
                    D_803B30C0[buttonIdx].angle = 0.0f;
                if (D_803B33C4)
                    D_803B30C0[buttonIdx].angle = 0.0f;
            }
            memcpy(prevPos, D_803B30C0[buttonIdx].position, 12);
            D_803B30C0[buttonIdx].position[0] = D_803B33D0 + -100.0f * D_803B33C8;
            D_803B30C0[buttonIdx].position[1] = buttonY;
            D_803B30C0[buttonIdx].position[2] = D_803B33DC;
            if (!D_803B33C4) {
                if (scrollSpeed < fabsf(D_803B30C0[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f) {
                    scrollSpeed = fabsf(D_803B30C0[buttonIdx].position[1] - prevPos[1]) * 60.0f / 10.0f;
                    scrollSpeed = scrollSpeed < 540.0f * D_803B33C8 ? scrollSpeed : 540.0f * D_803B33C8;
                    scrollSpeed = scrollSpeed > 180.0f * D_803B33C8 ? scrollSpeed : 180.0f * D_803B33C8;
                }
                if (D_803B30C0[buttonIdx].position[1] > prevPos[1] + scrollSpeed * D_8002EB94) {
                    D_803B30C0[buttonIdx].position[1] = prevPos[1] + scrollSpeed * D_8002EB94;
                    clearScrollSpeed = 0;
                } else if (D_803B30C0[buttonIdx].position[1] < prevPos[1] - scrollSpeed * D_8002EB94) {
                    D_803B30C0[buttonIdx].position[1] = prevPos[1] - scrollSpeed * D_8002EB94;
                    clearScrollSpeed = 0;
                }
            }
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[buttonIdx].matrix, D_803B33C8);
            func_800B5898(D_803B30C0[buttonIdx].angle - 1.5707964f, D_803B30C0[buttonIdx].matrix);
            if (D_803B30C0[buttonIdx].angle > 1.5707964f)
                tex = func_800B24EC(D_803B87C8, &texIdx, 0, *(volatile u8 *)&D_80140BDC - 1, 1);
            else
                tex = func_800B24EC(D_803B87D8, &texIdx, 0, *(volatile u8 *)&D_80140BDC - 1, 1);
            func_8008D870(D_803B30C0[buttonIdx].handle, (s32)tex, -1);
            if (D_803B30C0[buttonIdx].position[1] > D_803B33D4 + D_803B33CC * D_803B33C8
                || D_803B30C0[buttonIdx].position[1] < D_803B33D8 - D_803B33CC * D_803B33C8)
                D_803B30C0[buttonIdx].alpha = 0;
            else if (D_803B30C0[buttonIdx].position[1] > D_803B33D4)
                D_803B30C0[buttonIdx].alpha = 255.0f - (D_803B30C0[buttonIdx].position[1] - D_803B33D4) * 255 / (D_803B33CC * D_803B33C8);
            else if (D_803B30C0[buttonIdx].position[1] < D_803B33D8)
                D_803B30C0[buttonIdx].alpha = 255.0f - (D_803B33D8 - D_803B30C0[buttonIdx].position[1]) * 255 / (D_803B33CC * D_803B33C8);
            else
                D_803B30C0[buttonIdx].alpha = 255;
            if (!func_8039A448(buttonIdx))
                D_803B30C0[buttonIdx].alpha = 0;
            buttonColor.channel[3] = D_803B30C0[buttonIdx].alpha;
            func_8008E06C(D_803B30C0[buttonIdx].handle, (s32 *)&buttonColor);
            if (buttonIdx == 1) {
                D_803B30C0[4].angle = D_803B30C0[buttonIdx].angle;
                D_803B30C0[4].position[0] = D_803B33D0 + 100.0f * D_803B33C8;
                D_803B30C0[4].position[1] = D_803B30C0[buttonIdx].position[1];
                D_803B30C0[4].position[2] = D_803B30C0[buttonIdx].position[2];
                func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[4].matrix, D_803B33C8);
                func_800B5898(D_803B30C0[4].angle - 1.5707964f, D_803B30C0[4].matrix);
                func_8008D870(D_803B30C0[4].handle, (s32)tex, -1);
                D_803B30C0[4].alpha = D_803B30C0[buttonIdx].alpha;
                func_8008E06C(D_803B30C0[4].handle, (s32 *)&buttonColor);
                if (!func_8039A448(buttonIdx))
                    model_data_load(D_803B30C0[4].handle, 0, 15);
                else if (!D_803B30C0[buttonIdx].alpha)
                    model_data_load(D_803B30C0[4].handle, 0, 15);
                else
                    model_transform_setup(D_803B30C0[4].handle, 0, 15);
            } else if (buttonIdx == 2) {
                D_803B30C0[5].angle = D_803B30C0[buttonIdx].angle;
                D_803B30C0[5].position[0] = D_803B33D0 + 100.0f * D_803B33C8;
                D_803B30C0[5].position[1] = D_803B30C0[buttonIdx].position[1];
                D_803B30C0[5].position[2] = D_803B30C0[buttonIdx].position[2];
                func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[5].matrix, D_803B33C8);
                func_800B5898(D_803B30C0[5].angle - 1.5707964f, D_803B30C0[5].matrix);
                func_8008D870(D_803B30C0[5].handle, (s32)tex, -1);
                D_803B30C0[5].alpha = D_803B30C0[buttonIdx].alpha;
                func_8008E06C(D_803B30C0[5].handle, (s32 *)&buttonColor);
                if (!func_8039A448(buttonIdx))
                    model_data_load(D_803B30C0[5].handle, 0, 15);
                else if (!D_803B30C0[buttonIdx].alpha)
                    model_data_load(D_803B30C0[5].handle, 0, 15);
                else
                    model_transform_setup(D_803B30C0[5].handle, 0, 15);
            }
            if (!func_8039A448(buttonIdx))
                model_data_load(D_803B30C0[buttonIdx].handle, 0, 15);
            else if (!D_803B30C0[buttonIdx].alpha) {
                model_data_load(D_803B30C0[buttonIdx].handle, 0, 15);
                buttonY -= D_803B33CC * D_803B33C8;
            } else {
                model_transform_setup(D_803B30C0[buttonIdx].handle, 0, 15);
                buttonY -= D_803B33CC * D_803B33C8;
            }
        }
    }
    memcpy(prevPos, D_803B30C0[7].position, 12);
    D_803B30C0[7].angle += 6.2831855f * D_8002EB94;
    if (D_803B30C0[7].angle > 6.2831855f)
        D_803B30C0[7].angle -= 6.2831855f;
    D_803B30C0[7].position[0] = D_803B33D0 * D_803B33E0 / D_803B33DC + -140.0f * D_803B33C8;
    D_803B30C0[7].position[1] = selectedPos * D_803B33E0 / D_803B33DC;
    D_803B30C0[7].position[2] = D_803B33E0;
    if (!D_803B33C4) {
        if (cursorSpeed < fabsf(D_803B30C0[7].position[1] - prevPos[1]) * 60.0f / 10.0f) {
            cursorSpeed = fabsf(D_803B30C0[7].position[1] - prevPos[1]) * 60.0f / 10.0f;
            cursorSpeed = cursorSpeed < 540.0f * D_803B33C8 ? cursorSpeed : 540.0f * D_803B33C8;
            cursorSpeed = cursorSpeed > 180.0f * D_803B33C8 ? cursorSpeed : 180.0f * D_803B33C8;
        }
        if (D_803B30C0[7].position[1] > prevPos[1] + cursorSpeed * D_8002EB94)
            D_803B30C0[7].position[1] = prevPos[1] + cursorSpeed * D_8002EB94;
        else if (D_803B30C0[7].position[1] < prevPos[1] - cursorSpeed * D_8002EB94)
            D_803B30C0[7].position[1] = prevPos[1] - cursorSpeed * D_8002EB94;
        else
            cursorSpeed = 180.0f * D_803B33C8;
    }
    func_8008B32C((f32 (*)[3])D_8011418C, D_803B30C0[7].matrix, D_803B33C8 * 0.4f);
    func_800B5940(D_803B30C0[7].angle, D_803B30C0[7].matrix);
    func_800B5898(1.5707964f, D_803B30C0[7].matrix);
    if (D_803B3414 || D_803B3418) {
        model_data_load(D_803B30C0[7].handle, 0, 15);
        model_data_load(D_803B30C0[9].handle, 0, 15);
    } else {
        model_transform_setup(D_803B30C0[7].handle, 0, 15);
        model_transform_setup(D_803B30C0[9].handle, 0, 15);
    }
    if (clearScrollSpeed == 1)
        scrollSpeed = 0.0f;
    D_803B33C4 = 0;
}

typedef unsigned short u16;
typedef struct LanguageIndex {u8 other[108];u16 name_base;} LanguageIndex;
typedef struct Language {
    void *other0;
    u8 **text;
    void *other8;
    LanguageIndex *indices;
    u8 **names;
} Language;
typedef struct PlayerStats {u8 other0,rank;u16 count;u8 other4[72];} PlayerStats;
extern Language D_8017A4E0;
extern PlayerStats D_80154450[];
extern u8 D_803B8794[],D_803B8798[],D_803B879C[],D_803B87A0[];
extern s16 D_803B9A86,D_803B9A88,D_803B9A8A,D_803B9A8C;
extern void fcvt_wrapper(u8 *,const u8 *,...);
extern u8 *func_800BE4F0(u8 *,const u8 *);
extern void *object_create(s32);
extern s32 object_manager_update(u8 *,s16);
extern u8 object_type_byte2_get(void);

void func_8039B908(s32 player,u8 *destination)
{
    u8 number[32];
    func_800BE6A4(destination,D_8017A4E0.text[80]);
    fcvt_wrapper(number,D_803B87A0,D_80154450[player].count);
    func_800BE4F0(destination,number);
    func_800BE4F0(destination,D_8017A4E0.text[130]);
}

void func_8039B988(s32 player,u8 *destination)
{
    func_800BE6A4(destination,D_8017A4E0.text[77]);
    func_800BE4F0(destination,D_8017A4E0.names[D_80154450[player].rank + D_8017A4E0.indices->name_base]);
    func_800BE4F0(destination,D_803B879C);
    func_800BE4F0(destination,D_8017A4E0.text[129]);
}

void func_8039BA28(u8 *destination)
{
    u8 number[32];
    func_800BE6A4(destination,D_8017A4E0.text[72]);
    fcvt_wrapper(number,D_803B8798,D_80154640 - D_803BAD90->b9);
    func_800BE4F0(destination,number);
    func_800BE4F0(destination,D_80154640 - D_803BAD90->b9 != 1 ? D_8017A4E0.text[71] : D_8017A4E0.text[70]);
    func_800BE4F0(destination,D_8017A4E0.text[73]);
}

void func_8039BAE0(u8 *destination)
{
    u8 number[32];
    func_800BE6A4(destination,D_8017A4E0.text[74]);
    fcvt_wrapper(number,D_803B8794,D_803BAD90->b9);
    func_800BE4F0(destination,number);
    func_800BE4F0(destination,D_803BAD90->b9 != 1 ? D_8017A4E0.text[71] : D_8017A4E0.text[70]);
    func_800BE4F0(destination,D_8017A4E0.text[75]);
}

void func_8039BB80(void)
{
    u8 text[80];
    /* Measured unused scalar homes: 128 + 8*4 = native 160-byte frame. */
    s32 frame_home0, frame_home1, frame_home2, frame_home3;
    s32 frame_home4, frame_home5, frame_home6, frame_home7;
    s32 width;
    s32 maximum;
    s32 player;
    s32 i;
    D_803B9A86 = 0;
    D_803B9A88 = 0;
    if (D_803B3428) {
        object_create(11);
        if (D_803B3424) {
            D_803B9A88 += 35;
            width = object_manager_update(D_8017A4E0.text[67],-1);
            D_803B9A86 = width < D_803B9A86 ? D_803B9A86 : width;
            func_8039BAE0(text);
            width = object_manager_update(text,-1);
            D_803B9A86 = width < D_803B9A86 ? D_803B9A86 : width;
            func_8039BA28(text);
            width = object_manager_update(text,-1);
            D_803B9A86 = width < D_803B9A86 ? D_803B9A86 : width;
        } else {
            D_803B9A88 += 15;
            width = object_manager_update(D_8017A4E0.text[76],-1);
            D_803B9A86 = width < D_803B9A86 ? D_803B9A86 : width;
        }
        D_803B9A88 += object_type_byte2_get() + 30;
        maximum = 0;
        player = D_801543D4;
        for (i = 0; i < 2; i++) {
            func_800BE6A4(text,(u8 *)D_8014A118[player].owner->body + 20);
            width = object_manager_update(text,-1);
            if (width >= maximum) maximum = width;
            func_8039B988(player,text);
            width = object_manager_update(text,-1);
            if (width >= maximum) maximum = width;
            func_8039B908(player,text);
            width = object_manager_update(text,-1);
            if (width >= maximum) maximum = width;
            if (D_8014A108 == 1) break;
            player ^= 1;
        }
        if (D_8014A108 >= 2)
            D_803B9A86 = maximum + 160;
        else
            D_803B9A86 = maximum < D_803B9A86 ? D_803B9A86 : maximum;
    }
    D_803B9A86 += 20;
    D_803B9A8A = D_803B9A8C + (220 - D_803B9A8C - D_803B9A88) / 2;
}
/* Additional real image-A rendering context, appended to config_text/group.c. */
typedef struct CameraView { f32 uv[3][3],position[3]; u8 unknown30[104]; } CameraView;
extern CameraView D_80150B70[];
extern Color D_801146BC,D_801146C0;
extern u8 *D_801147C8[],*D_803B3454;
extern s32 D_80111784[];
extern u8 D_803B875C[],D_803B8764[],D_803B876C[],D_803B8774[],D_803B877C[],D_803B8784[],D_803B878C[],D_803B8790[],D_803B87A4[];
extern void render_helper(f32);
extern void func_800B669C(u32,u32);
extern void dispatch_handler(s32);
extern void brake_light_update(s32,f32 *,CameraView *,void *,s16 *);
extern s16 sound_pitch_diff_halved(void *,s16);
extern s16 object_bytes_sum_global(void);
extern void state_utility(s16,s16,void *);
extern void func_800ED66C(f32);
extern void func_800BEA3C(Color,Color);
extern void camera_auto_follow(s16,s16,s16,s16,s16,s16,u8 *);
extern void crowd_cheer_play(FiveParts *,s32,s32,s32,s32);
extern void music_fade(s16,s16,s16,u8 *);

/* A5658 renders the six competitor ranking/name/count entries. The selected
 * result-table selector is compared first in both equality expressions; the
 * natural for loop exposes its initializer and loop-header address setup on
 * the same compiler source line. Both are ordinary C with the same reads.
 *
 * Exact frame reconstruction: without the six unused scalar declarations,
 * stock ugen reports M=140, T=16, P=4 and frame 168. Six four-byte homes make
 * M=164 and frame 192, exactly retail. count moves +52 to +76, name[32] moves
 * +72 to +96, and number[64] moves +104 to +128. Buffer capacities unchanged.
 * These homes are a measured layout reconstruction, not recovered names or
 * a claim about their unknown original purpose. No padding array is used.
 */
void func_803A5658(void)
{
    u8 number[64],name[32];
    s32 i, rank, x, y;
    s32 count;
    s32 unused_home0, unused_home1, unused_home2;
    s32 unused_home3, unused_home4, unused_home5;
    object_create(10);
    dispatch_handler(1);
    object_type_byte2_get();
    count=6;
    for(i=0;i<count;i++) {
        if(D_803B9A84==D_803BAD90->b9)rank=D_80154450[i].rank;
        else {
            rank=D_80154450[i].other4[D_803B9A84];
            if(rank>=6) {
                object_create(10);
                dispatch_handler(1);
                state_utility(sound_pitch_diff_halved(D_8017A4E0.text[81],160),165,D_8017A4E0.text[81]);
                return;
            }
        }
        x=rank<3 ? 16 : 160;
        dispatch_handler(i<D_8014A108 ? 17 : 1);
        object_create(5);
        fcvt_wrapper(number,D_803B878C,rank+1);
        y=129+(rank%3)*24;
        state_utility(sound_pitch_diff_halved(number,x+6),y+1,number);
        object_create(11);
        if(i<D_8014A108)func_800BE6A4(name,(u8 *)D_8014A118[i].owner->body+20);
        else func_800BE6A4(name,D_801147C8[D_80154450[i].other0]);
        music_fade((s16)(x+15),y+2,120,name);
        fcvt_wrapper(number,D_803B8790,D_803B9A84==D_803BAD90->b9 ?
                     D_80154450[i].count : D_80154450[i].other4[24+D_803B9A84]);
        func_800BE4F0(number,D_8017A4E0.text[130]);
        state_utility((s16)(x+15),y+11,number);
    }
}

/* w36g: func_803A5948 = animText@circuitscr.c (MAT3 GC 0x800695FC), with animButtonText@circuitscr.c
 * (GC 0x80068850) and getFinishPlaceStr@circuitscr.c (GC 0x8006A000) inlined into it by umerge; their
 * deleted bodies are the caller-less `jr ra; nop` stubs func_803A5940 and func_803A5938 (non-static, off
 * the keep list, so each stub keeps its symbol). Direct typed port of reference/repos/mat3
 * decomp/src/GameSource/src/circuitscr.c animButtonText/animText/getFinishPlaceStr; N64 names below:
 * buttonInfo D_803B30C0 (tMenuButton[12], 64 B), buttonsInited D_803B33C0, gButtonScale D_803B33C8,
 * gEnterCircuitCode D_803B3414, gConfirmNewCircuit D_803B3418, gInvalidCode D_803B341C,
 * gWrongNumPlayers D_803B3420, gContinueEnabled D_803B3424, gStatsEnabled D_803B3428, gShowTrophy
 * D_803B342C, gCircuitMenuSelect D_803BACF8, newCircuitType D_803BAD38, gCircuit D_803BAD90 (b8 type,
 * b9 track), gCircuitCode D_803BAD98, gCodeSelect D_803B9A80, gReviewTrack D_803B9A84, gYes D_803BADC4,
 * pane D_803BACF0, gPasswordAlphabet D_80116FE4, gNumCircuitTracks D_80111784, gCircuitCars D_80154450,
 * flashColors D_801146BC/C0, gCamera D_80150B70, gText D_8017A4E0 (single +4, tab +0xC, taba +0x10);
 * HAL_SetTextDepth render_helper, TB_SetJust func_800B669C, ZOID_ProjectWindow brake_light_update,
 * HAL_SetFont object_create, TB_SetTextColor dispatch_handler, TB_CenterText sound_pitch_diff_halved,
 * TB_DrawText state_utility, TB_DrawTextScrollBox camera_auto_follow, TB_GetFontLineHeight
 * object_bytes_sum_global, HAL_SetTextTranslucency func_800ED66C, TB_FlashTextColors func_800BEA3C (tRGBA
 * by value on N64), TB_Sprintf fcvt_wrapper, TB_Strcpy func_800BE6A4, TB_Strcat func_800BE4F0,
 * HAL_GetTextWidth object_manager_update (MAT3 returns unsigned int: the macro below restores that type
 * for these calls without touching the shared s32 declaration the old members use), ChangePaneSize
 * crowd_cheer_play, getPointsStr func_8039B908, getPlaceStr func_8039B988, getToGoStr func_8039BA28,
 * getFinishedStr func_8039BAE0, drawReview func_803A5658.
 * N64 differences from the GC body, all read from the retail words: text indices and layout constants
 * (59/75/126/129/153, pane boxes), screenPos used unhalved, the 5-track cap when D_80156994 == 0, a
 * code-entry loop that steps x before the row test, and the del static at D_803B3454 (its "DEL" literal
 * is the image's D_803B8758, written as that symbol like the group's other literals). Shaping, measured:
 * - Local order i, str1, str, screenPos, x, y and a 192-byte str1 (GC: i, x, y, str1[64], str, screenPos)
 *   are the only layout that reproduces the retail frame (str1 +140, str +136, screenPos +132, x/y homes
 *   below; 344 bytes). No unused locals.
 * - getFinishPlaceStr is defined after animText (MAT3 file order); defined before it, umerge emits its
 *   parameter copy one instruction early.
 * - The two button-label strings go through `str` before TB_DrawText (as1 order of the argument loads).
 * - del is a block static as in MAT3: as an extern, uopt keeps &D_803B3454 in s0 across TB_CenterText.
 * Own data: three float literals (bits checked against the image at 0x803B9814..1C) and the del word
 * (0x803B3454 = 0x803B8758 in the image). */
extern u8 D_803B8758[];
#define HAL_GetTextWidth(s, n) ((u32)object_manager_update((s), (n)))
void func_803A5938(s32 p, u8 *str);

void func_803A5940(void)
{
    s32 i;
    u8 str1[192];
    u8 *str;
    s16 screenPos[2];
    s32 x;
    s32 y;

    render_helper(0.0f);
    func_800B669C(0, 1);
    brake_light_update(0, D_803B30C0[6].position, &D_80150B70[0], 0, screenPos);
    x = screenPos[0];
    y = screenPos[1];
    object_create(13);
    dispatch_handler(1);
    str = D_8017A4E0.text[42];
    state_utility(sound_pitch_diff_halved(str, x), y, str);
    if (D_803B3414) {
        static u8 *del = D_803B8758;
        func_800B669C(0, 3);
        object_create(11);
        y = 59;
        x = 115;
        dispatch_handler(1);
        state_utility(sound_pitch_diff_halved(D_8017A4E0.text[44], 160), y, D_8017A4E0.text[44]);
        y += 16;
        if (D_803B341C || D_803B3420) {
            dispatch_handler(5);
            state_utility(sound_pitch_diff_halved(D_8017A4E0.text[134], 160), y, D_8017A4E0.text[134]);
        }
        y += 16;
        dispatch_handler(12);
        str1[1] = 0;
        for (i = 0; i < 20; i++) {
            str1[0] = D_803BAD98[i];
            state_utility(sound_pitch_diff_halved(str1, x + 4), y, str1);
            if ((i + 1) % 10 == 0) {
                x = 115;
                y += 9;
            } else
                x += 9;
        }
        dispatch_handler(1);
        x = 94;
        y = 126;
        for (i = 0; i < 32; i++) {
            str1[0] = D_80116FE4[i];
            dispatch_handler(D_803B9A80 == i ? 22 : 1);
            state_utility(sound_pitch_diff_halved(str1, x + 5), y, str1);
            if ((i + 1) % 12 == 0) {
                x = 94;
                y += 11;
            } else
                x += 11;
        }
        x += 12;
        dispatch_handler(D_803B9A80 == -2 ? 22 : 1);
        state_utility(sound_pitch_diff_halved(del, x + 5), y, del);
        if (D_803BACF0)
            crowd_cheer_play(D_803BACF0, 80, 55, 240, 165);
    } else if (D_803B3418) {
        func_800B669C(1, 3);
        object_create(11);
        dispatch_handler(1);
        camera_auto_follow(160, 59, 140, 78, -1, 0, D_8017A4E0.text[96]);
        y = 59 + 5 * object_bytes_sum_global();
        dispatch_handler(D_803BADC4 ? 22 : 1);
        state_utility(145, y, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[24]]);
        dispatch_handler(D_803BADC4 ? 1 : 22);
        state_utility(175, y, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[24] + 1]);
        func_800B669C(0, 3);
        if (D_803BACF0)
            crowd_cheer_play(D_803BACF0, 60, 55, 260, 130);
    } else {
        object_create(11);
        for (i = 0; i < 4; i++) {
            if (D_803B30C0[i].angle > 0.6981317f && D_803B30C0[i].angle < 2.443461f)
                continue;
            if (!D_803B30C0[i].alpha)
                continue;
            func_800ED66C(D_803B30C0[i].alpha);
            brake_light_update(0, D_803B30C0[i].position, &D_80150B70[0], 0, screenPos);
            x = screenPos[0] - 35.0f * D_803B33C8;
            y = screenPos[1];
            if (D_803B30C0[i].angle < 1.5707964f) {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(22);
            }
            str = D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[43] + i];
            state_utility(x, y, str);
            if (i == 1) {
                brake_light_update(0, D_803B30C0[4].position, &D_80150B70[0], 0, screenPos);
                x = screenPos[0] - 35.0f * D_803B33C8;
                y = screenPos[1];
                if (D_803B9A84 == D_803BAD90->b9)
                    str = D_8017A4E0.text[176];
                else {
                    fcvt_wrapper(str1, D_803B875C, D_8017A4E0.text[82], D_803B9A84 + 1);
                    str = str1;
                }
                state_utility(x, y, str);
            }
            if (i == 2) {
                brake_light_update(0, D_803B30C0[5].position, &D_80150B70[0], 0, screenPos);
                x = screenPos[0] - 35.0f * D_803B33C8;
                y = screenPos[1];
                str = D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD38];
                state_utility(x, y, str);
            }
        }
        func_800ED66C(-1.0f);
        if (D_803B342C) {
            s16 width;
            str = str1;
            func_800B669C(0, 3);
            object_create(11);
            dispatch_handler(1);
            fcvt_wrapper(str, D_803B8764, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD90->b8], D_8017A4E0.text[76]);
            width = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), 153, str);
            y = 153 + object_bytes_sum_global();
            y += object_bytes_sum_global();
            func_803A5938(0, str);
            width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), y, str);
            y += object_bytes_sum_global();
            func_8039B908(0, str);
            width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), y, str);
            y += object_bytes_sum_global();
            crowd_cheer_play(D_803BACF0, 160 - width / 2 - 4, 149, 160 + width / 2 + 4, y + 4);
        } else if (D_803BACF8 == 2) {
            s32 numCircuitTracks;
            s16 width;
            str = str1;
            func_800B669C(0, 3);
            object_create(11);
            dispatch_handler(1);
            fcvt_wrapper(str, D_803B876C, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD38], D_8017A4E0.text[42]);
            width = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), 129, str);
            y = 129 + object_bytes_sum_global();
            numCircuitTracks = D_80111784[D_803BAD38];
            if (!D_80156994 && numCircuitTracks >= 5)
                numCircuitTracks = 5;
            fcvt_wrapper(str, D_803B8774, numCircuitTracks * (D_803BAD38 == 3 ? 4 : 2), D_8017A4E0.text[71]);
            width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
            state_utility(sound_pitch_diff_halved(str, 160), y, str);
            y += object_bytes_sum_global();
            crowd_cheer_play(D_803BACF0, 160 - width / 2 - 4, 125, 160 + width / 2 + 4, y + 4);
        } else if (D_803B3428) {
            str = str1;
            func_800B669C(0, 3);
            object_create(11);
            dispatch_handler(1);
            if (D_803BACF8 != 1) {
                s16 width;
                if (D_803B3424) {
                    fcvt_wrapper(str, D_803B877C, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD90->b8], D_8017A4E0.text[67]);
                    width = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
                    state_utility(sound_pitch_diff_halved(str, 160), 129, str);
                    y = 129 + object_bytes_sum_global();
                    func_8039BAE0(str);
                    width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
                    state_utility(sound_pitch_diff_halved(str, 160), y, str);
                    y += object_bytes_sum_global();
                    func_8039BA28(str);
                    width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
                    state_utility(sound_pitch_diff_halved(str, 160), y, str);
                    y += object_bytes_sum_global();
                } else {
                    fcvt_wrapper(str, D_803B8784, D_8017A4E0.names[((u16 *)D_8017A4E0.indices)[44] + D_803BAD90->b8], D_8017A4E0.text[76]);
                    width = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
                    state_utility(sound_pitch_diff_halved(str, 160), 129, str);
                    y = 129 + object_bytes_sum_global();
                }
                y += object_bytes_sum_global();
                func_8039B988(0, str);
                width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
                state_utility(sound_pitch_diff_halved(str, 160), y, str);
                y += object_bytes_sum_global();
                func_8039B908(0, str);
                width = width > HAL_GetTextWidth(str, -1) ? width : HAL_GetTextWidth(str, -1);
                state_utility(sound_pitch_diff_halved(str, 160), y, str);
                y += object_bytes_sum_global();
                crowd_cheer_play(D_803BACF0, 160 - width / 2 - 4, 125, 160 + width / 2 + 4, y + 4);
            } else {
                func_800B669C(0, 3);
                func_803A5658();
                if (D_803BACF0)
                    crowd_cheer_play(D_803BACF0, 12, 125, 308, 205);
            }
        } else
            crowd_cheer_play(D_803BACF0, -20, 100, -10, 120);
    }
    func_800B669C(0, 3);
    render_helper(-1.0f);
}

s32 func_803A5948(Blit *blit)
{
    if (D_803B33C0 == 1) {
        func_803A5940();
        return 1;
    }
    return 1;
}

void func_803A5938(s32 p, u8 *str)
{
    func_800BE6A4(str, D_8017A4E0.text[78]);
    func_800BE4F0(str, D_8017A4E0.names[D_8017A4E0.indices->name_base + D_80154450[p].rank]);
    func_800BE4F0(str, D_803B87A4);
    func_800BE4F0(str, D_8017A4E0.text[129]);
}

#undef HAL_GetTextWidth

/* B560 initializes twelve native 64-byte slots and six special transforms.
 * Float literals are authenticated against current-tree image A. Distinct
 * 55.2f/55.20f spellings preserve the two native constant webs.
 */
extern MenuObjectSlot D_803B30C0[12];
extern s32 D_803B33C0;
extern void *memcpy(void *,const void *,u32);
extern void func_8008B32C(f32 [3][3],f32 [3][3],f32);
extern void func_800B5898(f32,f32 [][3]);
extern void func_800B5940(f32,f32 [][3]);
extern Color D_803B3458;
extern char *D_803B309C[];
extern char D_803B87A8[],D_803B87B8[],D_803B87C0[],D_803B87C4[];
extern f32 D_803B9594,D_803B9598,D_803B959C,D_803B95A0,D_803B95A4;
extern f32 D_803B95A8,D_803B95AC,D_803B95B0;

void func_8039B560(void)
{
    Color color = D_803B3458;
    s32 i;
    s16 texture_index;
    f32 yaw;
    for (i = 0; i < 12; i++) {
        if (D_803B30C0[i].handle == -1) {
            memcpy(D_803B30C0[i].matrix, D_8011418C, 36);
            D_803B30C0[i].handle = sign_extend_call(
                string_copy_format(D_803B309C[D_803B30C0[i].other], 0, D_80140BDC - 1, 1),
                (u32)D_803B30C0[i].matrix, -1,
                (D_803B30C0[i].other == 0 || D_803B30C0[i].other == 1 || D_803B30C0[i].other == 2) ? 0x42000 : 0);
            func_8008E06C((s16)D_803B30C0[i].handle, (s32 *)&color);
        }
    }
    func_8008D870((s16)D_803B30C0[6].handle,
        (s32)func_800B24EC(D_803B87A8, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B30C0[6].matrix, D_8011418C, 36);
    yaw = -1.5707963705062866f;
    func_800B5898(yaw, D_803B30C0[6].matrix);
    D_803B30C0[6].position[0] = 0.0f;
    D_803B30C0[6].position[1] = 115.0f;
    D_803B30C0[6].position[2] = 200.0f;
    func_8008D870((s16)D_803B30C0[7].handle,
        (s32)func_800B24EC(D_803B87B8, &texture_index, 0, D_80140BDC - 1, 1), -1);
    func_8008D870((s16)D_803B30C0[8].handle,
        (s32)func_800B24EC(D_803B87C0, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B30C0[8].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B30C0[8].matrix);

    D_803B30C0[8].position[0] = 75.1f;
    D_803B30C0[8].position[1] = 55.2f;
    D_803B30C0[8].position[2] = 100.0f;
    func_8008B32C(D_803B30C0[8].matrix, D_803B30C0[8].matrix, 0.5f);
    memcpy(D_803B30C0[9].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B30C0[9].matrix);
    func_800B5940(3.1415927410125732f, D_803B30C0[9].matrix);
    D_803B30C0[9].position[0] = 87.1f;
    D_803B30C0[9].position[1] = 55.2f;
    D_803B30C0[9].position[2] = 100.0f;
    func_8008D870((s16)D_803B30C0[10].handle,
        (s32)func_800B24EC(D_803B87C4, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B30C0[10].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B30C0[10].matrix);

    D_803B30C0[10].position[0] = -75.1f;
    D_803B30C0[10].position[1] = 55.20f;
    D_803B30C0[10].position[2] = 100.0f;
    func_8008B32C(D_803B30C0[10].matrix, D_803B30C0[10].matrix, 0.5f);
    memcpy(D_803B30C0[11].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B30C0[11].matrix);
    D_803B30C0[11].position[0] = -87.1f;
    D_803B30C0[11].position[1] = 55.20f;
    D_803B30C0[11].position[2] = 100.0f;
    D_803B33C0 = 1;
    D_803B33C4 = 1;
    particle_velocity_set();
}
