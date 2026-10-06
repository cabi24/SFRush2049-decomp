/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Runtime A real-caller context for func_8039B120. Only B120 is a local
 * strict matching claim: 61/61 words at normal whole-program O3.
 * The complete native-derived B214 and BE48 callers are unclaimed NONMATCH
 * context. Their missing outer/private ABI context is not invented here.
 * A local returning sign_extend_call corrects the evidenced wrapper contract;
 * production source and locks are unchanged. See README for precise scope.
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad0[4]; u8 u4; u8 pad5[3]; s8 b8; s8 b9; } Cfg;
extern Cfg *D_803BAD90;
extern s8 D_80154640;
extern s8 D_803B3424;
extern u8 D_803B3428;
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

extern u32 D_80149784, D_8015694C;
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

typedef struct Player { u8 other[72]; u8 ***owner; } Player;
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
    s16 texture_index;
    for (i = 0; i < D_8014A108; i++) {
        object_byte71_set_sync(D_8014A118[i].owner, D_8014A110);
    }
    D_803BAD38 = 0;
    if (D_803BAD90->u4 && D_803BAD90->b9 == 0)
        D_803BAD90->u4 = 0;
    D_803B9A84 = D_803BAD90->b9;
    if (D_803BAD90->u4) {
        net_session_update();
        if (D_803BAD90->b9 >= D_80154640) {
            D_803B342C = 1;
            i = D_80154451;
            if (i < 3) {
                D_803B3448 = sign_extend_call(
                    string_copy_format(D_803B3430[i], 0, D_80140BDC - 1, 1),
                    (u32)D_803B9A50, -1, 0);
                D_803B344C = sign_extend_call(
                    string_copy_format(D_803B343C[i], 0, D_80140BDC - 1, 1),
                    (u32)D_8011418C, D_803B3448, 0x2C6084);
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
