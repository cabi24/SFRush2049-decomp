/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* B560 complete 936-byte object-initializer research, 14/234 words differ.
 * Real BE48 caller is kept context only, not a native-matching claim.
 * Main-blob wrapper stays external; constant data ownership remains unresolved.
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

extern void func_8039B120(void);

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

typedef struct MenuObjectSlot {
    s32 other,handle;
    f32 angle;
    f32 matrix[3][3];
    f32 position[3];
    u8 alpha,unaccessed[3];
} MenuObjectSlot;
typedef union Color {u32 word;u8 channel[4];} Color;
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
    f32 y;
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
    yaw = D_803B9594;
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
    y = D_803B9598;
    D_803B30C0[8].position[0] = D_803B959C;
    D_803B30C0[8].position[1] = y;
    D_803B30C0[8].position[2] = 100.0f;
    func_8008B32C(D_803B30C0[8].matrix, D_803B30C0[8].matrix, 0.5f);
    memcpy(D_803B30C0[9].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B30C0[9].matrix);
    func_800B5940(D_803B95A0, D_803B30C0[9].matrix);
    D_803B30C0[9].position[0] = D_803B95A4;
    D_803B30C0[9].position[1] = y;
    D_803B30C0[9].position[2] = 100.0f;
    func_8008D870((s16)D_803B30C0[10].handle,
        (s32)func_800B24EC(D_803B87C4, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B30C0[10].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B30C0[10].matrix);
    y = D_803B95A8;
    D_803B30C0[10].position[0] = D_803B95AC;
    D_803B30C0[10].position[1] = y;
    D_803B30C0[10].position[2] = 100.0f;
    func_8008B32C(D_803B30C0[10].matrix, D_803B30C0[10].matrix, 0.5f);
    memcpy(D_803B30C0[11].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B30C0[11].matrix);
    D_803B30C0[11].position[0] = D_803B95B0;
    D_803B30C0[11].position[1] = y;
    D_803B30C0[11].position[2] = 100.0f;
    D_803B33C0 = 1;
    D_803B33C4 = 1;
    particle_velocity_set();
}
