/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Runtime image B only: 0x8038C910..0x8038CA24, 276 bytes.
 * StartWeapon(slot) -- MAT3 (GameCube) weapons.c StartWeapon, GC 0x8005C7C0, 0xF4 bytes.
 * Direct typed port: v = &car[slot].visuals[<weapon>]; when the visual has no scene
 * object yet, fill the visual (index 0, callback AnimateWeapon = func_8038AA8C, timeStamp =
 * gSeconds, slot), create the car object with InitCarObj (locked label save_slot_valid,
 * func_800AF5E0), keep it in both gCarParts[slot].weap and v->objnum, and place it at
 * weaponOffsets[1][model[slot].body_type].
 * MAT3 evidence used as-is: `extern volatile float gSeconds` (the volatile makes IDO
 * materialise the address as lui/addiu + lwc1 0(reg), exactly as retail does; the earlier
 * pointer round-trip trick in research PR #364 is not needed), the chained result
 * assignment, the InitCarObj / ZOID_UpdateObject prototypes, weaponOffsets[8][13][3]
 * and gObjList[531] (weapon model). N64 differences: the weapon visual is visuals[20]
 * at car+0x2F0 (21 visuals at +0x110, GC has 22), tCarParts is 0x40 bytes with weap at
 * +0x18, model[] stride 0x808 with body_type at +8.
 * Field names from MAT3 tVisual; offsets from the N64 displacements.
 * Callees and data referenced by address (func_/D_ labels resolve by address):
 *   func_800AF5E0 = InitCarObj, func_8008D6FC = ZOID_UpdateObject,
 *   func_8038AA8C = AnimateWeapon, D_80152818 = gGameCar, D_80139320 = gCarParts,
 *   D_8014A250 = model, D_803943A4 = weaponOffsets, D_8002EB90 = gSeconds.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct tVisual tVisual;
struct tVisual {
    tVisual *next;      /* 0x00 */
    s16 index;          /* 0x04 */
    s16 objnum;         /* 0x06 */
    s16 slot;           /* 0x08 */
    u32 data;           /* 0x0C */
    f32 timeStamp;      /* 0x10 */
    void (*func)(tVisual *, s16); /* 0x14 */
};

typedef struct tCarData {
    u8 unknown000[0x110];
    tVisual visuals[21];            /* 0x110; weapon = visuals[20] at 0x2F0 */
    u8 unknown308[0x3b8 - 0x110 - 21 * 0x18];
} tCarData;                         /* 0x3B8 */

typedef struct tCarParts {
    s32 frame;                      /* 0x00 */
    u8 unknown04[0x14];
    s32 weap;                       /* 0x18 */
    u8 unknown1c[0x24];
} tCarParts;                        /* 0x40 */

typedef struct tModel {
    u8 unknown00[8];
    u8 body_type;                   /* 0x08 */
    u8 unknown09[0x7ff];
} tModel;                           /* 0x808 */

extern tCarData D_80152818[];
extern tCarParts D_80139320[];
extern tModel D_8014A250[];
extern f32 D_803943A4[8][13][3];
extern volatile f32 D_8002EB90;
extern u16 D_8014254C[];
extern void func_8038AA8C(tVisual *, s16);
extern s32 func_800AF5E0(s32, s16, s32, u32, s16, s16, s32);
extern void func_8008D6FC(s16, f32 *, f32 (*)[3]);

void func_8038C910(s16 slot)
{
    tVisual *v;

    v = &D_80152818[slot].visuals[20];
    if (v->objnum < 0) {
        v->index = 0;
        v->func = func_8038AA8C;
        v->timeStamp = D_8002EB90;
        v->slot = slot;
        v->objnum = D_80139320[slot].weap =
            func_800AF5E0(D_8014254C[531], 15, D_80139320[slot].frame, 0x2000, slot, -1, 1);
        func_8008D6FC(v->objnum, D_803943A4[1][D_8014A250[slot].body_type], 0);
    }
}
