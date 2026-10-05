/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sfx_stop (historical label): set or clear the texture of one car part.
 *   part = 0..12, car = 0..3 index the 64-byte car-part records at D_80139320 (13 per car,
 *   the 3328-byte block cleared by sfx_position_3d); tex = -1 .. 10.
 *   tex == -1: model_data_load(rec->model, 0, 1 << car) (hide the part for that car's view);
 *   otherwise look the texture name D_8011B438[tex] up with MBOX_FindTexture_Sub
 *   (func_800B24EC, warning on), apply it to the part's object (func_8008D870((s16)model, tex, -1))
 *   and model_transform_setup(rec->model, 0, 1 << car).
 * Arcade ancestor: not identified (N64 car-select part code).
 * Shaping quirks: the record is indexed 1-D as D_80139320[car * 13 + part] (uopt distributes the
 *   *64 and ugen spends one more temp than the 2-D [car][part] form, which shifts the temp ring);
 *   the car mask is a switch, not 1 << car; declaration order p, t, index, mask gives the 56-byte
 *   frame with mask at sp+44 and index at sp+46.  -O2 does not match.  Whole-program unit: EQUAL.
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;

typedef struct {
    u8 pad0[20];
    s32 model;      /* 20 */
    u8 pad24[40];
} CarPart;          /* 64 */

extern CarPart D_80139320[];
extern char *D_8011B438[];
extern volatile u8 D_80140BDC;

void *func_800B24EC(char *name, u16 *index, s8 lo, s8 hi, s32 err);
void func_8008D870(s16 object, void *tex, s32 mode);
void model_data_load(s32 model, s32 mode, s32 mask);
void model_transform_setup(s32 model, s32 mode, s32 mask);

void sfx_stop(s16 part, s16 car, s16 tex)
{
    CarPart *p;
    void *t;
    u16 index;
    s16 mask;

    if (tex < -1 || tex >= 11) {
        return;
    }
    switch (car) {
    case 0:
        mask = 1;
        break;
    case 1:
        mask = 2;
        break;
    case 2:
        mask = 4;
        break;
    case 3:
        mask = 8;
        break;
    }
    if (tex == -1) {
        model_data_load(D_80139320[car * 13 + part].model, 0, mask);
    } else {
        t = func_800B24EC(D_8011B438[tex], &index, 0, D_80140BDC - 1, 1);
        p = &D_80139320[car * 13 + part];
        func_8008D870(p->model, t, -1);
        model_transform_setup(p->model, 0, mask);
    }
}
