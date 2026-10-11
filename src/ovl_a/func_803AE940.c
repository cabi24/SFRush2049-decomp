/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Image A [0x803AE940,0x803AECC4), 900 bytes: front-end list-control
 * BLIT callback. Native-authoritative C reconstruction; no exact arcade
 * donor or original variable-name claim. Packed descriptor nibbles are
 * snapshotted before callbacks, while geometry and globals are read after
 * side-effecting helpers. Geometry is a four-point row, texture names have
 * two states per entry, and texture records have a 32-byte stride.
 * ResourceTables uses the accessed prefix independently established by
 * src/blob/groups/slot_sound/func_800A4E58.c. BLIT uses its accessed prefix.
 * Table bounds and actual resource contents are not inferred from strides.
 * Float-to-u32 behavior is C-defined only for representable finite values;
 * byte assignment then applies unsigned narrowing. Signed-halfword stores
 * follow the target implementation when the mathematical result is wider.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit {
    char *Name; void *Image; void *Info;
    s16 TexIndex, X, Y; u16 State;
    s16 Width, Height; u8 Alpha, Flip; s8 Hide; u8 Init;
    s16 Top, Bot, Left, Right, Color; u16 reserved26;
    s32 (*AnimFunc)(struct Blit *); u32 AnimID;
    u32 reserved30; u16 Texture;
} Blit;
typedef struct Point { s16 x, y; } Point;
typedef struct ResourceIndices { u16 unused, first; } ResourceIndices;
typedef struct ResourceTables {
    u16 count0; u32 *ptrs0; u16 count1, count2;
    ResourceIndices *indices; char **strings;
} ResourceTables;
typedef struct Texture { char pad[21]; u8 flags; char tail[10]; } Texture;
extern s32 D_803B7714;
extern s16 D_803BA878, D_803BA898, D_803BA85A, D_803BA8B0[];
extern s8 D_803BA908;
extern char *D_803B8294[][2];
extern Point D_803B9224[][4];
extern ResourceTables D_8017A4E0;
extern Texture D_80140BF0[];
extern s8 input_new_data_wrapper(Blit *, s32);
extern void func_800EF5B0(Blit *, char *, s32);
extern float func_800BEA30(void);
extern void func_800B42F0(s32);
extern s32 func_800B3FA4(char *, s32);
extern void Input_ApplyPadConfig(Blit *);
s32 func_803AE940(Blit *blt)
{
    s16 pulse = blt->AnimID & 15;
    s16 item = (blt->AnimID >> 4) & 15;
    s16 group = (blt->AnimID >> 8) & 15;
    s16 texture = (blt->AnimID >> 12) & 15;
    s16 active;
    if (input_new_data_wrapper(blt, D_803B7714 == 1)) return 1;
    if (group == 1) {
        if (input_new_data_wrapper(blt, item == 2)) return 1;
    }
    if (group == 0) {
        if (item == 2) {
            if (input_new_data_wrapper(blt, D_803BA878 == 0)) return 1;
        } else if (item == 3) {
            if (input_new_data_wrapper(blt, D_803BA898 - D_803BA878 < 5)) return 1;
        }
    }
    active = D_803BA908 || item >= 2;
    func_800EF5B0(blt, D_803B8294[texture][active], 1);
    if (pulse) blt->Alpha = (u32)(func_800BEA30() * 255.0f);
    blt->X = D_803B9224[group][item].x - blt->Width / 2;
    blt->Y = D_803B9224[group][item].y - blt->Height / 2;
    if (group == 0) {
        func_800B42F0(11);
        if (item == 0) {
            blt->X -= func_800B3FA4(D_8017A4E0.strings[D_803BA8B0[D_803BA85A] + D_8017A4E0.indices->first], -1);
        }
        if (item == 0 || item == 1) blt->Y += (D_803BA85A - D_803BA878) * 13;
    }
    if (item == 1) blt->Flip = 1;
    else if (item == 3) D_80140BF0[blt->Texture].flags |= 8;
    Input_ApplyPadConfig(blt);
    return 1;
}
